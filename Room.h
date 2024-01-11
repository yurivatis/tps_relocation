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
    void orientation(RoomNrOrientation ro) {ro_ = ro;}
    RoomNrOrientation orientation() {return ro_;}
    int rotation() {return rotation_;}
    void rotation(int r) {rotation_ = r;}
    virtual void redrawMates();
    bool serviceRoom() {return isService_;}
private:
    int capacity_;
    bool isService_;
    int nr_;
    QPolygonF coordinates_;
    bool dummy_;
    RoomNrOrientation ro_;
    int rotation_;
};


class Room163: public Room {
public:
    Room163(int nr, int capacity, QList<int>list);
    Room163(int nr, int capacity, QPolygonF coordinates);
    virtual void redrawMates();
};


class Room119: public Room {
public:
    Room119(int nr, int capacity, QList<int>list);
    Room119(int nr, int capacity, QPolygonF coordinates);
    virtual void redrawMates();
};
