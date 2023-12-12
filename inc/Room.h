#pragma once
#include <QString>
#include <QList>
#include <QPolygon>
#include <QPoint>
#include "Person.h"
class Room {
public:
    Room(int nr, int capacity, QList<int>list, bool dummy=false);
    Room(int nr, int capacity, QPolygon coordinates, bool dummy=false);
    int capacity(){return capacity_;}
    void capacity(int capacity) {capacity_ = capacity;}
    int nr(){return nr_;}
    void nr(int nr) {nr_ = nr;}
    bool inside(QPoint p);
    QPolygon coordinates() {return coordinates_;}
    QList<Person*> people() {return people_;}
    QPoint getCenter();
    bool dummy() {return dummy_;}
private:
    int capacity_;
    int nr_;
    QPolygon coordinates_;
    QList<Person*>people_;
    bool dummy_;
};
