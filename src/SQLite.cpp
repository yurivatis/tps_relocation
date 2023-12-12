#include "SQLite.h"
#include <QSqlQuery>
#include <QDebug>
#include <QColor>
#include <QSqlError>

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
}


SqlInterface::~SqlInterface()
{
    db_.close();
    instance_ = nullptr;
}


bool SqlInterface::customizeColors()
{
    bool ret = true;
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS colors (id integer, department_id integer, team_id integer, component_id integer,"
                  "red integer, greed integer, blue integer, alpha integer, "
                  "FOREIGN KEY (department_id) REFERENCES departments(id), FOREIGN KEY (team_id) REFERENCES teams(id), "
                  "FOREIGN KEY (component_id) REFERENCES components(id), "
                  "PRIMARY KEY(id));");
    ret = query.exec();
    qDebug() << "customize colors: " <<ret;
    RETURN_IF_QUERY_FAILED;
    return ret;
}


QColor SqlInterface::color(QString department, QString team, QString component)
{
    QColor color;
    int r, g, b, a;
    QSqlQuery query;
    query.prepare(QString("SELECT red, green, blue, alpha FROM colors WHERE department_id = (SELECT id FROM departments WHERE departments.department = %1), "
                          "team_id = (SELECT id FROM team WHERE teams.team = %2),"
                          "component_id = (SELECT id FROM component WHERE components.component = %3)").arg(department, team, component));
    query.exec();
    while(query.next()) {
        r = query.value(0).toInt();
        g = query.value(1).toInt();
        b = query.value(2).toInt();
        a = query.value(3).toInt();
    }
    color = QColor(r, g, b, a);
    return color;
}


bool SqlInterface::color(QString department, QString team, QString component, QColor color)
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
                          "(SELECT id FROM componentes WHERE componentes.component = '%3'),:red, :green, :blue, :apha);").arg(department, team, component));
    query.bindValue(":red", r);
    query.bindValue(":green", g);
    query.bindValue(":blue", b);
    query.bindValue(":alpha", a);
    ret = query.exec();
    return ret;
}


bool SqlInterface::import(const QString cvs)
{
    bool ret = true;
    QFile f(cvs);
    QString req;
    if(db_.isOpen()) {
        db_.close();
    }
    QFile::remove(dbName_);
    db_.open();
    customizeColors();

    QStringList locations, teams, components, departments;

    //create tables
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS locations (id integer, location VARCHAR, PRIMARY KEY(id));");
    ret = query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS departments (id integer, department VARCHAR, PRIMARY KEY(id));");
    ret = query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS teams (id integer, team VARCHAR, PRIMARY KEY(id));");
    ret = query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS components (id integer, component VARCHAR, PRIMARY KEY(id));");
    ret = query.exec();
    query.prepare("CREATE TABLE IF NOT EXISTS people (id integer, name VARCHAR, surname VARCHAR, "
                  "location_id integer, department_id integer, team_id integer, component_id integer, role VARCHAR, room integer, "
                  "FOREIGN KEY (location_id) REFERENCES locations(id), FOREIGN KEY (department_id) REFERENCES departments(id),"
                  "FOREIGN KEY (team_id) REFERENCES teams(id), FOREIGN KEY (component_id) REFERENCES components(id), PRIMARY KEY(id) );");
    ret = query.exec();
    //get locations
    f.setFileName(cvs);
    if(f.open (QIODevice::ReadOnly| QIODevice::Text)) {
        QSqlQuery que;
        QTextStream ts (&f);
        while(!ts.atEnd()) {
            QString tmp;
            QStringList line = ts.readLine().split('\n');
            for(int i=0; i<line.length ();++i) {
                QStringList values = line.at(i).split(',');
                for(int j = 0; j < values.length() - 1; ++j) {
                    tmp = values.at(j);
                    switch (j) {
                        case 2: // locations
                        if(!locations.contains(tmp)) {
                            locations.append(tmp);
                        }
                        break;
                        case 3: // departments
                        if(!departments.contains(tmp)) {
                            departments.append(tmp);
                        }
                        break;
                        case 4: // teams
                        if(!teams.contains(tmp)) {
                            teams.append(tmp);
                        }
                        break;
                        case 6: // components
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

    if(f.open (QIODevice::ReadOnly| QIODevice::Text)){
        QTextStream ts (&f);

        //Travel through the csv file
        while(!ts.atEnd()){
            QString req = "INSERT INTO people (name, surname, location_id, department_id, team_id, role, component_id, room) VALUES (";
            // split every line
            QStringList line = ts.readLine().split('\n');
            /*for every values on a line,
                   append it to the INSERT request*/
//            QStringList values = line.split(',');
            for(int i = 0; i < line.length(); ++i) {
                QStringList values = line.at(i).split(',');
                for(int j = 0; j < values.length(); ++j) {
                    switch(j) {
                        case 0: // name
                        case 1: // surname
                        case 5: // role
                            req.append('\'');
                            req.append(values.at(j));
                            req.append('\'');
                            req.append(",");
                            break;
                        case 7: // room
//                            req.append('\'');
                            req.append(values.at(j));
//                            req.append('\'');
                            break;
                        case 2: // location
                            req.append(QString("(select id from locations where locations.location = '%1'),").arg(values.at(j)));
                            break;
                        case 3: // department
                            req.append(QString("(select id from departments where departments.department = '%1'),").arg(values.at(j)));
                            break;
                        case 4: // teams
                            req.append(QString("(select id from teams where teams.team = '%1'),").arg(values.at(j)));
                            break;
                        case 6: // component
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
        if(values.at(i).length()) {
            req = QString("INSERT INTO %1 (%2) VALUES (").arg(tableName, columnName);
            req.append('\'');
            req.append(values.at(i));
            req.append('\'');
            req.append(");"); // close the "VALUES([...]" with a ");"
            query.exec(req);
            RETURN_IF_QUERY_FAILED;
        }
    }
    return true;
}


void SqlInterface::displayPeople()
{

}


void SqlInterface::displayRooms()
{

}


void SqlInterface::displayPeopleInRoom(int room)
{

}


QStringList SqlInterface::departments()
{
    if(db_.isOpen() == false) {
        db_.open();
    }
    QStringList res;
    QSqlQuery query;
    bool ret = true;
    QString req;
    req = QString("SELECT department FROM departments ORDER BY department ASC");
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


QStringList SqlInterface::teams(const QString department)
{
    if(db_.isOpen() == false) {
        db_.open();
    }
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("select distinct team from teams INNER JOIN people ON people.team_id = teams.id INNER JOIN departments ON people.department_id = departments.id WHERE departments.department = '%1' ORDER BY team ASC").arg(department);
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}


QStringList SqlInterface::components(const QString team)
{
    if(db_.isOpen() == false) {
        db_.open();
    }
    QStringList res;
    QSqlQuery query;
    QString req;
    req = QString("select distinct component from components INNER JOIN people ON people.component_id = components.id INNER JOIN teams ON people.team_id = teams.id WHERE teams.team ='%1' ORDER BY component ASC").arg(team);
    query.exec(req);
    while (query.next()) {
        QString d = query.value(0).toString();
        res.append(d);
    }
    return res;
}
