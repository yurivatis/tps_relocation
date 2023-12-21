#include "Room.h"
#include <QDebug>

Room::Room(int nr, int capacity, QPolygonF coordinates, RoomOrientation ro, bool dummy): capacity_(capacity), nr_(nr) {
    coordinates_ = coordinates;
    dummy_ = dummy;
    ro_ = ro;
}

Room::Room(int nr, int capacity, QList<int>list, RoomOrientation ro, bool dummy): capacity_(capacity), nr_(nr) {
    if(list.size() % 2 != 0) {
        qDebug() << "could not add odd number of its to room coordinates";
        return;
    }
    for(int i = 0; i < list.size(); i+=2) {
        coordinates_ << QPoint(list.at(i), list.at(i+1));
    }
    dummy_ = dummy;
    ro_ = ro;
}


QPoint Room::getCenter()
{
    int x = 0, y = 0;
    if(coordinates_.size() == 0) {
        return QPoint(0, 0);
    }
    if(capacity_ == 0 || ro_ == Orientation::CENTER) {
        y = (coordinates_.at(0).y() + coordinates_.at(1).y()) / 2;
    } else if(ro_ == Orientation::DOWN) {
        y = coordinates_.at(0).y() + 15;
    } else if(ro_ == Orientation::UP) {
        y = coordinates_.at(1).y() - 5;
    } else {
        y = coordinates_.at(1).y();
    }
    x = (coordinates_.at(coordinates_.size()-1).x() + coordinates_.at(0).x()) / 2;

    return QPoint(x - 5, y);
}


bool Room::addPerson(Person* person)
{
    foreach(Person *p, people_) {
        if(person->name() == p->name() && person->surname() == p->surname()) {
            return false;
        }
    }
    people_.append(person);
    return true;
}


bool Room::removePerson(Person* person)
{
    foreach(Person *p, people_) {
        if(person->name() == p->name() && person->surname() == p->surname()) {
            people_.removeOne(person);
            return true;
        }
    }
    qDebug() << person->surname() << " " << person->name() << " not found in Room " << nr_;
    return true;
}

