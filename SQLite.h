#pragma once

#include <QStringList>
#include <QSqlDatabase>
#include <QFile>
#include "Person.h"

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
    bool customizeRooms();
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
    bool exportToDb(QList<Person*>people);
    QColor readColor(const QString &department, const QString &team, const QString &component, const QColor defColor);
    bool writeColor(const QString &department, const QString &team, QString const &component, const QColor color);
    bool clearColorTable();
    int colorEntries();
    void people(QList<Person*>&list);
    int room(int nr, int capacity, bool force = false);
    QStringList getColorTable();
    QStringList departments();
    QStringList teams(const QString department);
    QStringList components(const QString team);
    QStringList components(const QString department, const QString team);
 };
