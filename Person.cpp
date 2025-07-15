#include "Person.h"
#include <QRegularExpression>

Person::Person(const QString &n, const QString &s, const QString &l, const QString &d, const QString &t, const QString &r, const QString &c, int rm ) {
    name(n);
    surname(s);
    role(r);
    department(d);
    team(t);
    room(rm);
    location(l);
    component(c);
    displayNameFirstFull_=false;
}


Person::Person()
{
    displayNameFirstFull_=false;
}


QString Person::displayName()
{
    QString surName, lastName;
    if(displayNameFirstFull_) {
        surName = longNameToDisplay(surname());
        lastName = shortNameToDisplay(name());
    } else {
        surName = shortNameToDisplay(surname());
        lastName = longNameToDisplay(name());
    }
    return surName + " " + lastName;
}


QString Person::shortNameToDisplay(QString val)
{
    QString retName;
    QStringList nameToDisplay = val.split(QRegularExpression(" "));
    int words = nameToDisplay.count();
    for(int i = 0; i < words; i++) {
        retName += QString(nameToDisplay.at(i).at(0)) + ".";
    }
    return retName;
}


QString Person::longNameToDisplay(QString val)
{
    int minLetters = 3;
    QString retName;
    QStringList nameToDisplay = val.split(QRegularExpression(" "));
    int words = nameToDisplay.count();
    if(words == 1) {
        return val;
    }
    retName = nameToDisplay.at(0);
    if(nameToDisplay.at(0).length() < minLetters) {
        retName += " " + nameToDisplay.at(1);
    } else {
        retName += QString(" ") + nameToDisplay.at(1).at(0) + ".";
    }
    for(int i = 2; i < words; i++) {
        retName += QString(nameToDisplay.at(i).at(0)) + ".";
    }
    return retName;
}


QString Person::displayFullName()
{
    return QString(surname() + " " + name());
}


void Person::moveTo(int x, int y)
{
    tmpCoordinates_.clear();
    for(int i = 0; i < coordinates_.size(); i++) {
        QPoint p0;
        p0.setX(coordinates_.at(i).x() + x - x_);
        p0.setY(coordinates_.at(i).y() + y - y_);
        tmpCoordinates_ << p0;
    }
}


void Person::clear()
{
    tmpCoordinates_ = coordinates_;
    y_ = x_ = 0;
}


bool Person::isLead()
{
    if(role().contains("Team Lead") || role().contains("Teamlead") || role().contains("Teamleiter") || role().contains("Leitung") || role().contains("Head") || role().contains("TL")) {
        return true;
    }
    // exceptions
    if(name() == "Götz") {
        return true;
    }
    return false;
}
