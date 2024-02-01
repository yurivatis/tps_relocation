#pragma once
#include <QString>
#include <QPolygonF>
#include <QColor>

class Person {
public:
    Person(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, int=0);
    Person();
    const QString name() {return name_;}
    void name(const QString name) {name_ = name;}
    const QString surname() {return surname_;}
    void surname(const QString surname) {surname_ = surname;}
    const QString role() {return role_;}
    void role(const QString role) {role_ = role;}
    const QString department() {return department_;}
    void department(const QString department) {department_ = department;}
    const QString team() {return team_;}
    void team(const QString team) {team_ = team;}
    int room() {return room_;}
    void room(const int room) {room_ = room; modifiedRoom_ = room;}
    QString location() {return location_;}
    void location(const QString location) {location_ = location;}
    QString component() {return component_;}
    void component(const QString component) {component_ = component;}
    QColor color() {return color_;}
    void color(QColor color) {color_ = color;}
    QPolygonF tmpcoordinates() {return tmpCoordinates_;}
    void tmpcoordinates(QPolygonF coordinates) {tmpCoordinates_ = coordinates;}
    QPolygonF coordinates() {return coordinates_;}
    void coordinates(QPolygonF coordinates) {coordinates_ = coordinates; tmpCoordinates_ = coordinates;}
    void offset(int x, int y) {x_ = x; y_ = y;}
    void clear();
    int offset_x(){return x_;}
    int offset_y(){return y_;}
    void moveTo(int x, int y);
    QString displayName();
    QString displayFullName();
    bool isLead();
    int modified() {return modifiedRoom_;}
    void modified(int m) {modifiedRoom_ = m;}
    void setDisplayFirstNameFull(bool b) {displayNameFirstFull_ = b;}
private:
    QString shortNameToDisplay(QString);
    QString longNameToDisplay(QString);
    QString name_;
    QString surname_;
    QString role_;
    QString department_;
    QString team_;
    QString location_;
    QString component_;
    QPolygonF coordinates_;
    QPolygonF tmpCoordinates_;
    QColor color_;
    int room_;
    int modifiedRoom_;
    int x_;
    int y_;
    bool displayNameFirstFull_;
};
