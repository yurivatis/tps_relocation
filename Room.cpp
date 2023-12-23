#include "Room.h"
#include <QDebug>

Room::Room(int nr, int capacity, QPolygonF coordinates, RoomNrOrientation ro, int rotation, bool dummy): capacity_(capacity), nr_(nr) {
    coordinates_ = coordinates;
    dummy_ = dummy;
    ro_ = ro;
    rotation_ = rotation;
}

Room::Room(int nr, int capacity, QList<int>list, RoomNrOrientation ro, int rotation, bool dummy): capacity_(capacity), nr_(nr) {
    if(list.size() % 2 != 0) {
        qDebug() << "could not add odd number of its to room coordinates";
        return;
    }
    for(int i = 0; i < list.size(); i+=2) {
        coordinates_ << QPoint(list.at(i), list.at(i+1));
    }
    dummy_ = dummy;
    ro_ = ro;
    rotation_ = rotation;
}


QPoint Room::getCenter()
{
    int x = 0, y = 0;
    if(coordinates_.size() == 0) {
        return QPoint(0, 0);
    }
    if(rotation_ == -90) {
        if(capacity_ == 0 || ro_ == Orientation::CENTER) {
            y = (coordinates_.at(0).y() + coordinates_.at(1).y()) / 2 + 5;
        } else if(ro_ == Orientation::DOWN) {
            y = coordinates_.at(0).y() + 15;
        } else if(ro_ == Orientation::UP) {
            y = coordinates_.at(1).y() - 5;
        } else {
            y = coordinates_.at(1).y();
        }
        if(coordinates_.size() == 4) {
            x = (coordinates_.at(coordinates_.size()-1).x() + coordinates_.at(0).x()) / 2;
        } else {
            for(int i = 0; i < coordinates_.size(); i++) {
                x += coordinates_.at(i).x();
            }
            x /= coordinates_.size();
        }
        return QPoint(x - 8, y);
    } else {
        if(capacity_ == 0 || ro_ == Orientation::CENTER) {
            for(int i = 0; i < coordinates_.size(); i++) {
                x += coordinates_.at(i).x();
                y += coordinates_.at(i).y();
            }
            x /= coordinates_.size();
            y /= coordinates_.size();
        } else if(ro_ == Orientation::RIGHT) {
            if(coordinates_.size() > 2) {
                x = (coordinates_.at(1).x() + coordinates_.at(2).x()) / 2;
                y = (coordinates_.at(1).y() + coordinates_.at(2).y()) / 2;
            }
        } else if(ro_ == Orientation::LEFT) {
            if(coordinates_.size() > 3) {
                x = (coordinates_.at(0).x() + coordinates_.at(3).x()) / 2 - 35;
                y = (coordinates_.at(0).y() + coordinates_.at(3).y()) / 2 + 5;
            }
        } else if(ro_ == Orientation::UP) {
            if(coordinates_.size() > 1) {
                x = (coordinates_.at(0).x() + coordinates_.at(1).x()) / 2;
                y = (coordinates_.at(0).y() + coordinates_.at(1).y()) / 2;
            }
        } else {// DOWN
            if(coordinates_.size() > 1) {
                x = (coordinates_.at(0).x() + coordinates_.at(coordinates_.size() - 1).x()) / 2;
                y = (coordinates_.at(0).y() + coordinates_.at(coordinates_.size() - 1).y()) / 2;
            }
        }
        return QPoint(x + 5, y);
    }
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

