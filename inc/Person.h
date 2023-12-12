#pragma once
#include <QString>
class Person {
public:
    Person(const QString, const QString, const QString, const QString, const QString, const QString, const QString, int=0);
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
    void room(const int room) {room_ = room;}
    QString location() {return location_;}
    void location(const QString location) {location_ = location;}
    QString component() {return component_;}
    void component(const QString component) {component_ = component;}
private:
    QString name_;
    QString surname_;
    QString role_;
    QString department_;
    QString team_;
    QString location_;
    QString component_;
    int room_;
};
