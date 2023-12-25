#include "MemberModel.h"
#include "SQLite.h"
#include "constants.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MemberModel::MemberModel(int rows, int cols, QObject *parent)
    : QStandardItemModel(rows, cols, parent)
{
    columns_ = cols;
    rows_ = rows;
    people_ = nullptr;
}


int MemberModel::rowCount(const QModelIndex & /*parent*/) const
{
    return rows_;
}


int MemberModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns_;
}


QVariant MemberModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == (int)MemberColumns::NAME) {
            return QString("Name");
        } else if (section == (int)MemberColumns::ROOM) {
            return QString("Room");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return QString("%1").arg(section + 1);
    }
    return QVariant();
}


QVariant MemberModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    int cur_row = index.row();
    int cur_col = index.column();
    switch(role) {
        case Qt::DisplayRole:
        {
            if(people_ == nullptr || people_->size() <= cur_row) {
                value = "";
                break;
            } else if (cur_col == (int)MemberColumns::ROOM) {
                value = QString::number(people_->at(cur_row)->modified());
            } else if(cur_col == (int)MemberColumns::NAME) {
                value = people_->at(cur_row)->displayFullName();
            }
            break;
        }
    }
    return value;
}


int MemberModel::restore(QList<Person*>*people)
{
    people_ = people;
    rows_ = people->size();
    setRowCount(rows_);

    return rows_;
}
