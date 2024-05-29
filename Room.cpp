#include "Room.h"
#include <QDebug>
#include "SQLite.h"

Room::Room(int nr, int cap, QPolygonF coordinates, RoomNrOrientation ro, int rotation, bool dummy): capacity_(cap), nr_(nr) {
    coordinates_ = coordinates;
    dummy_ = dummy;
    ro_ = ro;
    rotation_ = rotation;
    if(cap <= 0) {
        isService_ = true;
    } else {
        isService_ = false;
    }
    if(nr != 0 && dummy_ == false) {
        capacity(SqlInterface::getInstance()->room(nr, cap));
    }
}


Room::Room(int nr, int cap, QList<int>list, RoomNrOrientation ro, int rotation, bool dummy): capacity_(cap), nr_(nr) {
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
    if(cap <= 0) {
        isService_ = true;
    } else {
        isService_ = false;
    }
    if(nr != 0 && dummy_ == false) {
        capacity(SqlInterface::getInstance()->room(nr, cap));
    }
}


QPoint Room::getCenter()
{
    int x = 0, y = 0;
    if(coordinates_.size() == 0) {
        return QPoint(0, 0);
    }
    if(rotation_ == -90) {
        if(serviceRoom() || ro_ == Orientation::CENTER) {
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
            if(nr() >= 10) {
                x-= 10;
            }
        } else {
            for(int i = 0; i < coordinates_.size(); i++) {
                x += coordinates_.at(i).x();
            }
            x /= coordinates_.size();
        }
        return QPoint(x - 8, y);
    } else {
        if(serviceRoom() || ro_ == Orientation::CENTER) {
            for(int i = 0; i < coordinates_.size(); i++) {
                x += coordinates_.at(i).x();
                y += coordinates_.at(i).y();
            }
            x /= coordinates_.size();
            y /= coordinates_.size();
        } else if(ro_ == Orientation::RIGHT) {
            if(coordinates_.size() > 2) {
                x = (coordinates_.at(1).x() + coordinates_.at(2).x()) / 2 + 10;
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
        return QPoint(x - 5, y);
    }
}


bool Room::addPerson(Person* person)
{
    foreach(Person *p, people_) {
        if(person->name() == p->name() && person->surname() == p->surname()) {
            qDebug() << person->surname() << " " << person->name() << " already in Room " << nr_;
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
    return false;
}


void Room::redrawMates()
{
    int sz = people_.size();
    if(sz == 0) {
        return;
    }
    QPolygonF roomPlg = coordinates();
    float stepX = (roomPlg.at(roomPlg.size() - 1).x() - roomPlg.at(0).x()) / sz;
    float stepY = (roomPlg.at(roomPlg.size() - 1).y() - roomPlg.at(0).y()) / sz;

    for(int i = 0; i < sz; i++) {
        QPolygonF personPlg;
        QPoint p0, p1, p2, p3;
        p0.setX(roomPlg.at(0).x() + i * stepX);
        p0.setY(roomPlg.at(0).y() + i * stepY);
        personPlg << p0;
        p1.setX(roomPlg.at(1).x() + i * stepX);
        p1.setY(roomPlg.at(1).y() + i * stepY);
        personPlg << p1;
        p2.setX(roomPlg.at(1).x() + (i+1) * stepX);
        p2.setY(roomPlg.at(1).y() + (i+1) * stepY);
        personPlg << p2;
        p3.setX(roomPlg.at(0).x() + (i+1) * stepX);
        p3.setY(roomPlg.at(0).y() + (i+1) * stepY);
        personPlg << p3;
        people_.at(i)->coordinates(personPlg);
    }
}


Room163::Room163(int nr, int capacity, QList<int>list) : Room(nr, capacity, list)
{
    this->nr((int)163);
    orientation(Orientation::DOWN);
}


Room163::Room163(int nr, int capacity, QPolygonF coordinates) : Room (nr, capacity, coordinates)
{
    this->nr((int)163);
    orientation(Orientation::DOWN);
}


void Room163::redrawMates()
{
    float offsetY = 0;
    int sz = people_.size();
    if(sz == 0) {
        return;
    }
    QPolygonF roomPlg = coordinates();
    float stepX = (roomPlg.at(roomPlg.size() - 1).x() - roomPlg.at(0).x()) / sz;
    float stepY = (roomPlg.at(roomPlg.size() - 1).y() - roomPlg.at(0).y()) / sz;
    if(sz == 1) {
        offsetY = (roomPlg.at(2).y() - roomPlg.at(3).y());
    } else {
        offsetY = (roomPlg.at(2).y() - roomPlg.at(3).y()) / (sz - 1);
    }
    // sort by length of name due to non-symmetrical form
    QList <Person*> tmp;
    for(int i = 0; i < sz; i++) {
        tmp.append(people_.at(i));
    }
    people_.clear();
    for(int i = 0; i < sz; i++) {
        int pos = 0;
        int len = 0;
        for(int j = 0; j < tmp.size(); j++) {
            if(tmp.at(j)->displayName().length() > len) {
                len = tmp.at(j)->displayName().length();
                pos = j;
            }
        }
        people_.append(tmp.at(pos));
        tmp.removeAt(pos);
    }

    for(int i = 0; i < sz; i++) {
        QPolygonF personPlg;
        QPoint p0, p1, p2, p3;
        p0.setX(roomPlg.at(0).x() + i * stepX);
        p0.setY(roomPlg.at(0).y() + i * stepY);
        personPlg << p0;
        p1.setX(roomPlg.at(1).x() + i * stepX);
        if(i > 0) {
            p1.setY(roomPlg.at(1).y() + (i - 1) * (stepY - offsetY));
        } else {
            p1.setY(roomPlg.at(1).y() + i * (stepY - offsetY));
        }
        personPlg << p1;
        p2.setX(roomPlg.at(3).x());
        p2.setY(roomPlg.at(3).y());
        p2.setX(roomPlg.at(1).x() + (i+1) * stepX);
        if(i == 0 && sz != 1) {
            p2.setY(roomPlg.at(1).y() + (stepY));
        } else if(i == 0 && sz == 1) {
            p2.setY(roomPlg.at(1).y() + (stepY - offsetY));
        } else {
            p2.setY(roomPlg.at(1).y() + (i) * (stepY - offsetY));
        }
        personPlg << p2;
        p3.setX(roomPlg.at(0).x() + (i+1) * stepX);
        p3.setY(roomPlg.at(0).y() + (i+1) * stepY);
        personPlg << p3;
        // find i.th max by length and assign to the corresponding one
        people_.at(i)->coordinates(personPlg);
    }
}


Room119::Room119(int nr, int capacity, QPolygonF coordinates) : Room(nr, capacity, coordinates)
{
    orientation(Orientation::RIGHT);
    rotation(-20);
}


Room119::Room119(int nr, int capacity, QList<int>list): Room(nr, capacity, list)
{
    orientation(Orientation::RIGHT);
    rotation(-20);
}


void Room119::redrawMates()
{
    int sz = people_.size();
    if(sz == 0) {
        return;
    }
    QPolygonF roomPlg = coordinates();
    float stepX = (roomPlg.at(roomPlg.size() - 1).x() - roomPlg.at(0).x()) / sz;
    float stepY = (roomPlg.at(roomPlg.size() - 1).y() - roomPlg.at(0).y()) / sz;
    for(int i = 0; i < sz; i++) {
        QPolygonF personPlg;
        QPoint p0, p1, p2, p3;
        p0.setX(roomPlg.at(0).x() + i * stepX);
        p0.setY(roomPlg.at(0).y() + i * stepY);
        personPlg << p0;
        p1.setX(roomPlg.at(1).x() + i * stepX);
        p1.setY(roomPlg.at(1).y() + i * stepY);
        personPlg << p1;
        if(i == sz - 1) {
            p2.setX(roomPlg.at(3).x());
            p2.setY(roomPlg.at(3).y());
        } else {
            p2.setX(roomPlg.at(1).x() + (i+1) * stepX);
            if(i == 0) {
                p2.setY(roomPlg.at(1).y() + (i+1) * (stepY));
            } else {
                p2.setY(roomPlg.at(1).y() + (i+1) * (stepY));
            }
        }
        personPlg << p2;
        p3.setX(roomPlg.at(0).x() + (i+1) * stepX);
        p3.setY(roomPlg.at(0).y() + (i+1) * stepY);
        personPlg << p3;
        people_.at(i)->coordinates(personPlg);
    }
}
