#include "MemberModel.h"
#include "constants.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>

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
        if (section == (int)MemberColumns::FULL_NAME) {
            return QString("Name");
        } else if (section == (int)MemberColumns::LOCATION) {
            return QString("Location");
        } else if (section == (int)MemberColumns::DEPARTMENT) {
            return QString("Department");
        } else if (section == (int)MemberColumns::TEAM) {
            return QString("Team");
        } else if (section == (int)MemberColumns::COMPONENT) {
            return QString("Component");
        } else if (section == (int)MemberColumns::ROLE) {
            return QString("Role");
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
            if(index.row() < 0 || index.column() < 0) {
                qDebug() << "QModelIndex index invalid";
                value = "";
                break;
            } else if(people_ == nullptr || people_->size() <= cur_row) {
                value = "";
                break;
            } else if (cur_col == (int)MemberColumns::LOCATION) {
                value = QString(people_->at(cur_row)->location());
            } else if (cur_col == (int)MemberColumns::DEPARTMENT) {
                value = QString(people_->at(cur_row)->department());
            } else if (cur_col == (int)MemberColumns::TEAM) {
                value = QString(people_->at(cur_row)->team());
            } else if (cur_col == (int)MemberColumns::COMPONENT) {
                value = QString(people_->at(cur_row)->component());
            } else if (cur_col == (int)MemberColumns::ROLE) {
                value = QString(people_->at(cur_row)->role());
            } else if (cur_col == (int)MemberColumns::ROOM) {
                value = QString::number(people_->at(cur_row)->modified());
            } else if(cur_col == (int)MemberColumns::FULL_NAME) {
                value = people_->at(cur_row)->displayFullName();
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


int MemberModel::restore(QList<Person*>*people)
{
    people_ = people;
    rows_ = people->size();
    setRowCount(rows_);
    return rows_;
}


SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :  QSortFilterProxyModel(parent)
{

}

QVariant SortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    auto mappedIndex = mapToSource( index );
    return sourceModel()->data( mappedIndex, role );
}


bool SortFilterProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{
    for(int i = 0; i < sourceModel()->columnCount(); i ++)
    {
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        if(sourceModel()->data(index).toString().toLower().trimmed().contains(filterRegularExpression())) {
            return true;
        }
    }
    return false;
}


void SortFilterProxyModel::setSearch(const QString &val)
{
    QRegularExpression regExp(val);
    this->setFilterRegularExpression(regExp);
}

