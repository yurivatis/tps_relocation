#include "Person.h"
Person::Person(const QString n, const QString s, const QString l, const QString d, const QString t, const QString r, const QString c, int rm ) {
    name(n);
    surname(s);
    role(r);
    department(d);
    team(t);
    room(rm);
    location(l);
    component(c);
}


