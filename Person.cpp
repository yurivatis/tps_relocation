#include "Person.h"

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
    if(displayNameFirstFull_) {
        return surname().split(' ').at(0) + " " + QString(name().at(0)) + ".";
    } else {
        return QString(surname().at(0)) + ". " + name().split(' ').at(0);
    }
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
    if(role().contains("Team Lead") || role().contains("Teamleiter")) {
        return true;
    }
    // exceptions
    if(name() == "Habib" || name() == "Gerken" || name() == "Götz") {
        return true;
    }
    return false;
}
