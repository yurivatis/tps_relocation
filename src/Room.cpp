#include "Room.h"
#include <QDebug>

Room::Room(int nr, int capacity, QPolygon coordinates, bool dummy): capacity_(capacity), nr_(nr) {
    coordinates_ = coordinates;
    dummy_ = dummy;
}

Room::Room(int nr, int capacity, QList<int>list, bool dummy): capacity_(capacity), nr_(nr) {
    if(list.size() % 2 != 0) {
        qDebug() << "could not add odd number of its to room coordinates";
        return;
    }
    for(int i = 0; i < list.size(); i+=2) {
        coordinates_ << QPoint(list.at(i), list.at(i+1));
    }
    dummy_ = dummy;
}


QPoint Room::getCenter()
{
    int x = 0, y = 0;
    if(coordinates_.size() == 0) {
        return QPoint(0, 0);
    }
    for(int i = 0; i < coordinates_.size(); ++i) {
        x+=coordinates_.at(i).x();
        y+=coordinates_.at(i).y();
    }
    x /= coordinates_.size();
    y /= coordinates_.size();
    return QPoint(x - 10, y + 5);
}
