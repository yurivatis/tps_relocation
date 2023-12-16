#include "ColorModel.h"
#include "CComboBox.h"
#include "constants.h"
#include <QDebug>

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
//        case Qt::BackgroundRole:
//            if(index.column()==4 ) {
//                QBrush Background(Qt::red);
//                return Background;
//            }
//        break;
        case Qt::DisplayRole:
        {
            if(list_.size() > 0) {
                foreach(ModelValue item, list_) {
                    if(index.column() == item.col && index.row() == item.row) {
                        qDebug() << index.row() << " : " << index.column() << " : " << item.value;
                        value = item.value;
                        break;
                    }
                }
            }
            
        }
    }
    return value;
}


void ColorModel::setComboBox(const QModelIndex& index, const QString value)
{
    qDebug () << "set combo box: " << index.column();
    CComboBox* obj = static_cast<CComboBox*>(sender());

    ModelValue mv = {index.row(), index.column(), ""};
    if(obj && obj->currentIndex() >= 0) {
        mv.value = value;
    }
    qDebug () << "value read";
    foreach(ModelValue item, list_) {
        if(item.col == mv.col && item.row == mv.row) {
            qDebug() << index.row() << " : " << index.column() << " : " << "";
            return;
        }
    }
    list_.append(mv);
    qDebug() << "appending: " << list_.size();
}
