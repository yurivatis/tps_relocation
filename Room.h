#pragma once
#include <QString>
#include <QList>
#include <QPolygonF>
#include <QPoint>
#include "Person.h"
#include "constants.h"

class Room {
public:
    Room(int nr, int capacity, QList<int>list, RoomNrOrientation ro = Orientation::DOWN, int rotation = -90, bool dummy=false);
    Room(int nr, int capacity, QPolygonF coordinates, RoomNrOrientation ro = Orientation::DOWN, int rotation = -90, bool dummy=false);
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
    RoomNrOrientation orientation() {return ro_;}
    int rotation() {return rotation_;}
    void rotation(int r) {rotation_ = r;}
private:
    int capacity_;
    int nr_;
    QPolygonF coordinates_;
    bool dummy_;
    RoomNrOrientation ro_;
    int rotation_;
};