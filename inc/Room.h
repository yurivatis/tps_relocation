#pragma once
#include <QString>
#include <QList>
#include <QPolygonF>
#include <QPoint>
#include "Person.h"
#include "constants.h"

class Room {
public:
    Room(int nr, int capacity, QList<int>list, RoomOrientation ro = Orientation::DOWN, bool dummy=false);
    Room(int nr, int capacity, QPolygonF coordinates, RoomOrientation ro = Orientation::DOWN, bool dummy=false);
    int capacity(){return capacity_;}
    void capacity(int capacity) {capacity_ = capacity;}
    int nr(){return nr_;}
    void nr(int nr) {nr_ = nr;}
    bool inside(QPoint p);
    QPolygonF coordinates() {return coordinates_;}
    QPoint getCenter();
    bool addPerson(Person *);
    bool removePerson(Person *);
    bool dummy() {return dummy_;}
    QList<Person*>people_;
    RoomOrientation orientation() {return ro_;}
private:
    int capacity_;
    int nr_;
    QPolygonF coordinates_;
    bool dummy_;
    RoomOrientation ro_;
};
