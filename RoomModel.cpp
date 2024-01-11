#include "RoomModel.h"
#include "constants.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>

RoomModel::RoomModel(int rows, int cols, QList<Room*>*r, QObject *parent)
    : QStandardItemModel(rows, cols, parent)
{
    columns_ = cols;
    rows_ = rows;
    rooms_ = r;
}


int RoomModel::rowCount(const QModelIndex & /*parent*/) const
{
    return rows_;
}


int RoomModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns_;
}


QVariant RoomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == (int)RoomColumns::NUMBER) {
            return QString("Number");
        } else if (section == (int)RoomColumns::CAPACITY) {
            return QString("Capacity");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return QString("%1").arg(section + 1);
    }
    return QVariant();
}


QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    int cur_row = index.row();
    int cur_col = index.column();
    switch(role) {
        case Qt::DisplayRole:
        {
            if(index.row() < 0 || index.column() < 0) {
                qDebug() << "QModelIndex index invalid";
                value = "";
                break;
            } else if(rooms_ == nullptr || rooms_->size() <= cur_row) {
                value = "";
                break;
            } else if (cur_col == (int)RoomColumns::NUMBER) {
                value = rooms_->at(cur_row)->nr();
            } else if (cur_col == (int)RoomColumns::CAPACITY) {
                value = rooms_->at(cur_row)->capacity();
            }
            break;
        }
        case Qt::BackgroundRole:
        {
            return QColor(Qt::white);
        }
    }
    return value;
}


bool SortRoomProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, (int)RoomColumns::NUMBER, sourceParent);
    if(sourceModel()->data(index).toString() != "0") {
        return true;
    }
    return false;
}
