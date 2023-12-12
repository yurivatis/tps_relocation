#pragma once

#include <QStringList>
#include <QSqlDatabase>
#include <QDate>
#include <QFile>

#define RETURN_IF_QUERY_FAILED \
     if(ret == false) { \
        qWarning() << query.lastError(); \
        return false; \
 }

class SqlInterface
{
private:
    bool importTable(QString tableName, QString columnName, QStringList &values);
    bool customizeColors();
    /* Here will be the instance stored. */
    static SqlInterface* instance_;
    QString lastError_;
    QSqlDatabase db_;
    /* Private constructor to prevent instancing. */
    SqlInterface();
    QString dbName_;

 public:
    virtual ~SqlInterface();
    /* Static access method. */
    static SqlInterface* getInstance();
    QString lastError() {return lastError_;}
    bool import(const QString cvs);
    void displayPeople();
    void displayRooms();
    void displayPeopleInRoom(int room);
    QColor color(QString department, QString team, QString component);
    bool color(QString department, QString team, QString component, QColor color);
    QStringList departments();
    QStringList teams(const QString department);
    QStringList components(const QString team);
 };
