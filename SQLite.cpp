#include "SQLite.h"
#include "constants.h"
#include <QSqlQuery>
#include <QColor>
#include <QSqlError>
#include <QRegularExpression>

#define DB_NAME "data.db"


SqlInterface* SqlInterface::instance_ = nullptr;

SqlInterface* SqlInterface::getInstance()
{
    if(instance_ == nullptr) {
        instance_ = new SqlInterface();
    }
    return instance_;
}


SqlInterface::SqlInterface()
{
    dbName_ = DB_NAME;
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName_);
    db_.open();
    prevMajor_ = -1;
    prevMinor_ = -1;
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS version (type TEXT, number integer)");
    query.exec();
    query.clear();
    query.prepare(QString("SELECT number FROM version WHERE type = 'MAJOR'"));
    query.exec();
    while(query.next()) {
        prevMajor_ = query.value(0).toInt();
        break;
    }
    query.clear();
    query.prepare(QString("SELECT number FROM version WHERE type = 'MINOR'"));
    query.exec();
    while(query.next()) {
        prevMinor_ = query.value(0).toInt();
        break;
    }
    query.clear();
    if(prevMajor_ != -1) {
        query.prepare("UPDATE version set number = :major WHERE type = 'MAJOR'");
    } else {
        query.prepare("INSERT INTO version (type, number) VALUES ('MAJOR', :major)");
    }
    query.bindValue(":major", TPS_RELOC_MAJOR);
    query.exec();
    query.clear();
    if(prevMinor_ != -1) {
        query.prepare("UPDATE version set number = :minor WHERE type = 'MINOR'");
    } else {
        query.prepare("INSERT INTO version (type, number) VALUES ('MINOR', :minor)");
    }
    query.bindValue(":minor", TPS_RELOC_MINOR);
    query.exec();
    query.clear();
    query.prepare("CREATE TABLE IF NOT EXISTS settings (name VARCHAR, value integer);");
    query.exec();
    query.prepare("INSERT OR REPLACE INTO settings (name, value) VALUES (:name, :value)");
    query.bindValue(":name", "display_last_name");
    query.bindValue(":value", 1);
    query.exec();
    query.clear();
    checkCompatible();
}


SqlInterface::~SqlInterface()
{
    db_.close();
    instance_ = nullptr;
}


void SqlInterface::checkCompatible()
{
    compatible_ = (prevMajor_ == TPS_RELOC_MAJOR);
}


bool SqlInterface::customizeColors()
{
    bool ret = true;
    QSqlQuery query;
    if(compatible_ == false) {
        query.prepare("DROP TABLE IF EXISTS colors;");
        query.exec();
        query.clear();
    }
    query.prepare("CREATE TABLE IF NOT EXISTS colors (id integer, department_id integer, team_id integer, component_id integer,"
                  "red integer, green integer, blue integer, alpha integer, "
                  "FOREIGN KEY (department_id) REFERENCES departments(id), FOREIGN KEY (team_id) REFERENCES teams(id), "
                  "FOREIGN KEY (component_id) REFERENCES components(id), "
                  "PRIMARY KEY(id));");
    ret = query.exec();
    RETURN_IF_QUERY_FAILED;
    return ret;
}


bool SqlInterface::customizeRooms()
{
    bool ret = true;
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS rooms (room_nr integer, room_capacity integer, "
                  "PRIMARY KEY(room_nr));");
    ret = query.exec();
    RETURN_IF_QUERY_FAILED;
    return ret;
}


int SqlInterface::room(int nr, int capacity, bool force)
{
    int cap = -1;
    customizeRooms();

    QSqlQuery query;
    if(force) {
        query.prepare("INSERT INTO rooms(room_nr, room_capacity) VALUES (:nr, :capacity) "
                      "ON CONFLICT(room_nr) DO UPDATE SET room_capacity = :capacity WHERE room_nr = :nr");
    } else {
        query.prepare("INSERT OR IGNORE INTO rooms(room_nr, room_capacity) VALUES (:nr, :capacity) ");
    }
    query.bindValue(":nr", nr);
    query.bindValue(":capacity", capacity);
    query.exec();
    QString str = QString("SELECT room_capacity FROM rooms WHERE room_nr = %1").arg(nr);
    query.prepare(str);
    query.exec();
    query.first();
    cap=query.value(0).toInt();
    return cap;
}


QColor SqlInterface::readColor(const QString &department, const QString &team, const QString &component, const QColor defColor)
{
    QColor color = defColor;
    QColor defCompColor = defColor;
    int r, g, b, a;
    QSqlQuery query;
    QString q;
    q = QString("SELECT red, green, blue, alpha FROM colors WHERE department_id = (SELECT id FROM departments WHERE departments.department = '%1') AND "
                          "team_id = (SELECT id FROM teams WHERE teams.team = '%2') AND "
                          "component_id = (SELECT id FROM components WHERE components.component = '');").arg(department).arg(team);
    query.prepare(q);
    query.exec();
    while(query.next()) {
        r = query.value(0).toInt();
        g = query.value(1).toInt();
        b = query.value(2).toInt();
        a = query.value(3).toInt();
        defCompColor = QColor(r, g, b, a);
        color = defCompColor;
    }
    query.clear();
    if(component != "") {
        q = QString("SELECT red, green, blue, alpha FROM colors WHERE department_id = (SELECT id FROM departments WHERE departments.department = '%1') AND "
                          "team_id = (SELECT id FROM teams WHERE teams.team = '%2') AND "
                          "component_id = (SELECT id FROM components WHERE components.component = '%3');").arg(department).arg(team).arg(component);
        query.prepare(q);
        query.exec();
        while(query.next()) {
            r = query.value(0).toInt();
            g = query.value(1).toInt();
            b = query.value(2).toInt();
            a = query.value(3).toInt();
            color = QColor(r, g, b, a);
        }
    }
    return color;
}


bool SqlInterface::writeColor(const QString &department, const QString &team, const QString &component, QColor color)
{
    int r,g,b,a;
    QSqlQuery query;
    bool ret = true;

    r = color.red();
    b = color.blue();
    g = color.green();
    a = color.alpha();
    query.prepare(QString("INSERT INTO colors (department_id, team_id, component_id, red, green, blue, alpha) VALUES( "
                          "(SELECT id FROM departments WHERE departments.department = '%1'),"
                          "(SELECT id FROM teams WHERE teams.team = '%2'),"
                          "(SELECT id FROM components WHERE components.component = '%3'),:red, :green, :blue, :alpha);").arg(department).arg(team).arg(component));
    query.bindValue(":red", r);
    query.bindValue(":green", g);
    query.bindValue(":blue", b);
    query.bindValue(":alpha", a);
    ret = query.exec();
    return ret;
}


bool SqlInterface::clearColorTable()
{
    bool ret;
    QSqlQuery query;
    query.prepare("DELETE FROM colors;");
    ret = query.exec();
    return ret;
}


QStringList SqlInterface::getColorTable()
{
    QStringList sl;
    QString val;
    QSqlQuery query;
    query.prepare(QString("SELECT department, team, component, red, green, blue, alpha "
                          "FROM departments INNER JOIN colors ON departments.id = colors.department_id "
                          "INNER JOIN teams ON colors.team_id = teams.id INNER JOIN components ON colors.component_id = components.id;"));
    query.exec();
    while (query.next()) {
        for(int i = 0; i < 7; i++) {
            sl.append(query.value(i).toString());
        }
    }
    return sl;
}


int SqlInterface::colorEntries()
{
    QSqlQuery query;
    query.prepare(QString("select count(*) from colors;"));
    query.exec();
    if(query.first()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}


void SqlInterface::people(QList<Person*>&list)
{
    Person *p;
    QSqlQuery query;
    query.prepare(QString("SELECT surname, name, location, department, team, component, role, room from people "
                          "INNER JOIN locations ON locations.id = people.location_id "
                          "INNER JOIN departments ON departments.id = people.department_id "
                          "INNER JOIN teams on teams.id = people.team_id "
                          "LEFT JOIN components ON components.id = people.component_id "));
    query.exec();
    while (query.next()) {
        p = new Person();
        p->surname(query.value(0).toString());
        p->name(query.value(1).toString());
        p->location(query.value(2).toString());
        p->department(query.value(3).toString());
        p->team(query.value(4).toString());
        p->component(query.value(5).toString());
        p->role(query.value(6).toString());
        p->room(query.value(7).toInt());
        list.append(p);
    }
    query.clear();
}


bool SqlInterface::exportToDb(QList<Person *> people)
{
    bool ret;
    QSqlQuery query;
    foreach(Person *p, people) {
        QString s = QString("UPDATE people SET room = :room WHERE name = '%1' and surname = '%2';").arg(p->name(), p->surname());
        query.prepare(s);
        query.bindValue(":room", p->room());
        ret = query.exec();
        if(ret == false) {
            return false;
        }
    }
    return true;
}


bool SqlInterface::import(const QString cvs)
{
    bool ret = true;
    QFile f(cvs);
    QString req;

    QStringList locations, teams, components, departments;

    //create tables
    QSqlQuery query;
    query.prepare("DROP TABLE IF EXISTS people;");
    query.exec();
    query.prepare("DROP TABLE IF EXISTS components;");
    query.exec();
    query.prepare("DROP TABLE IF EXISTS teams;");
    query.exec();
    query.prepare("DROP TABLE IF EXISTS departments;");
    query.exec();
    query.prepare("DROP TABLE IF EXISTS locations;");
    query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS locations (id integer, location VARCHAR, PRIMARY KEY(id));");
    query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS departments (id integer, department VARCHAR, PRIMARY KEY(id));");
    query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS teams (id integer, team VARCHAR, PRIMARY KEY(id));");
    query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS components (id integer, component VARCHAR, PRIMARY KEY(id));");
    query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS people (id integer, surname VARCHAR, name VARCHAR, "
                  "location_id integer, department_id integer, team_id integer, component_id integer, role VARCHAR, room integer, "
                  "FOREIGN KEY (location_id) REFERENCES locations(id), FOREIGN KEY (department_id) REFERENCES departments(id),"
                  "FOREIGN KEY (team_id) REFERENCES teams(id), FOREIGN KEY (component_id) REFERENCES components(id), PRIMARY KEY(id) );");
    query.exec();

    //get locations
    f.setFileName(cvs);
    QRegularExpression separator(CSV_SEPARATOR);
    if(f.open (QIODevice::ReadOnly| QIODevice::Text)) {
        QTextStream ts (&f);
        //add empty component
        components.append("");
        while(!ts.atEnd()) {
            QString tmp;
            QStringList line = ts.readLine().split('\n');
            for(int i=0; i<line.length ();++i) {
                QStringList values = line.at(i).split(separator);
                for(int j = 0; j < values.length() - 1; ++j) {
                    tmp = values.at(j);
                    switch (j) {
                        case (int)SQLitePeopleColumns::LOCATION: // locations
                        if(!locations.contains(tmp)) {
                            locations.append(tmp);
                        }
                        break;
                        case (int)SQLitePeopleColumns::DEPARTMENT: // departments
                        if(!departments.contains(tmp)) {
                            departments.append(tmp);
                        }
                        break;
                        case (int)SQLitePeopleColumns::TEAM: // teams
                        if(!teams.contains(tmp)) {
                            teams.append(tmp);
                        }
                        break;
                        case (int)SQLitePeopleColumns::COMPONENT: // components
                        if(!components.contains(tmp)) {
                            components.append(tmp);
                        }
                        break;
                    }
                }
            }
        }
    }
    importTable("departments", "department", departments);
    importTable("locations", "location", locations);
    importTable("teams", "team", teams);
    importTable("components", "component", components);
    f.close();

    customizeColors();
    customizeRooms();

    if(f.open (QIODevice::ReadOnly| QIODevice::Text)) {
        QTextStream ts (&f);
        //Travel through the csv file
        while(!ts.atEnd()){
            QString req = "INSERT INTO people (surname, name, location_id, department_id, team_id, role, component_id, room) VALUES (";
            // split every line
            QStringList line = ts.readLine().split('\n');
            /*for every values on a line,
                   append it to the INSERT request*/
//            QStringList values = line.split(',');
            for(int i = 0; i < line.length(); ++i) {
                QStringList values = line.at(i).split(separator);
                for(int j = 0; j < values.length(); ++j) {
                    switch(j) {
                        case (int)SQLitePeopleColumns::SURNAME: // surname
                        case (int)SQLitePeopleColumns::NAME: // name
                        case (int)SQLitePeopleColumns::ROLE: // role
                            req.append('\'');
                            req.append(values.at(j));
                            req.append('\'');
                            req.append(",");
                            break;
                        case (int)SQLitePeopleColumns::ROOM: // room
                            if(values.at(j).isEmpty()) {
                                req.append('0');
                            } else {
                                req.append(values.at(j));
                            }
                            break;
                        case (int)SQLitePeopleColumns::LOCATION: // location
                            req.append(QString("(select id from locations where locations.location = '%1'),").arg(values.at(j)));
                            break;
                        case (int)SQLitePeopleColumns::DEPARTMENT: // department
                            req.append(QString("(select id from departments where departments.department = '%1'),").arg(values.at(j)));
                            break;
                        case (int)SQLitePeopleColumns::TEAM: // teams
                            req.append(QString("(select id from teams where teams.team = '%1'),").arg(values.at(j)));
                            break;
                        case (int)SQLitePeopleColumns::COMPONENT: // component
                            req.append(QString("(select id from components where components.component = '%1'),").arg(values.at(j)));
                            break;
                    }
                }
            }
            req.append(");"); // close the "VALUES([...]" with a ");"
            ret = query.exec(req);
            RETURN_IF_QUERY_FAILED;
        }
    }
    return ret;
}


bool SqlInterface::importTable(QString tableName, QString columnName, QStringList &values)
{
    QString req;
    QSqlQuery query;
    bool ret = true;
    for(int i = 0; i < values.length(); i++) {
        req = QString("INSERT INTO %1 (%2) VALUES (").arg(tableName, columnName);
        req.append('\'');
        req.append(values.at(i));
        req.append('\'');
        req.append(");"); // close the "VALUES([...]" with a ");"
        query.exec(req);
        RETURN_IF_QUERY_FAILED;
    }
    return true;
}


QStringList SqlInterface::departments()
{
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("SELECT department FROM departments ORDER BY department DESC");
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


QStringList SqlInterface::teams(const QString department)
{
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("SELECT DISTINCT team FROM teams INNER JOIN people ON people.team_id = teams.id "
                  "INNER JOIN departments ON people.department_id = departments.id WHERE departments.department = '%1' ORDER BY team ASC").arg(department);
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


QStringList SqlInterface::components(const QString team)
{
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("SELECT DISTINCT component FROM components INNER JOIN people ON people.component_id = components.id "
                  "INNER JOIN teams ON people.team_id = teams.id WHERE teams.team ='%1' ORDER BY component ASC").arg(team);
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


QStringList SqlInterface::components(const QString department, const QString team)
{
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("SELECT DISTINCT component FROM components INNER JOIN people ON people.component_id = components.id INNER JOIN teams ON people.team_id = teams.id "
                  "INNER JOIN departments ON people.department_id = departments.id "
                  "WHERE teams.team ='%1' AND departments.department='%2' ORDER BY component ASC").arg(team).arg(department);
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


void SqlInterface::displayLastName(bool val)
{
    QSqlQuery query;
    QString req = QString("UPDATE settings SET value=:val WHERE name = 'display_last_name'");
    query.prepare(req);
    query.bindValue(":val", val?1:0);
    query.exec();
}


bool SqlInterface::displayLastName()
{
    int val = 1;
    QSqlQuery query;
    QString req = QString("SELECT value FROM settings WHERE name = 'display_last_name'");
    query.prepare(req);
    query.exec();
    query.first();
    val=query.value(0).toInt();
    return val > 0;
}
