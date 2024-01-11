#pragma once
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QVariant>
#include <QList>
#include "Room.h"

class RoomModel : public QStandardItemModel {
    Q_OBJECT
public:
    RoomModel(int rows, int cols, QList<Room*>*r, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QList<Room*>* rooms() {return rooms_;}
private:
    int rows_;
    int columns_;
    QList<Room*> *rooms_;

signals:
    void oUpdated(int, int);
};


class SortRoomProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    SortRoomProxyModel(QObject * = nullptr) {}
    QList<Room*>* rooms() {return rooms_;}
    void rooms(QList<Room*>* r) {rooms_ = r;}
private:
    QList<Room*> *rooms_;
};
