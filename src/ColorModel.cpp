#include "ColorModel.h"
#include "constants.h"

ColorModel::ColorModel(int rows, int cols, QObject *parent)
    : QStandardItemModel(rows, cols, parent)
{
    rows_ = rows;
    columns_ = cols;
}


int ColorModel::rowCount(const QModelIndex & /*parent*/) const
{
   return rows_;
}


int ColorModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns_;
}



QVariant ColorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == (int)Column::DEPARTMENT) {
            return QString("Department");
        } else if (section == (int)Column::TEAM) {
            return QString("Team");
        } else if (section == (int)Column::COMPONENT) {
            return QString("Component");
        } else if (section == (int)Column::COLOR) {
            return QString("Color");
        } else if (section == (int)Column::ADD_NEW) {
            return QString("Add row");
        } else if (section == (int)Column::REMOVE) {
            return QString("Remove row");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return QString("%1").arg(section + 1);
    }
    return QVariant();
}


QVariant ColorModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch(role) {
        case Qt::BackgroundRole:
            if(index.column()==4 ) {
                QBrush Background(Qt::red);
                return Background;
            }
        break;
        case Qt::DisplayRole:
        {
        }
    }
    return value;
}

/*
if (role == Qt::BackgroundColorRole) {
if (index.column() == _colorColumn) {
        return Qt::blue;
    }
}
*/
