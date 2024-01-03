#pragma once
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QVariant>
#include <QList>
#include "Person.h"

class MemberModel : public QStandardItemModel {
    Q_OBJECT
public:
    MemberModel(int rows, int cols, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QList<Person*>* people() {return people_;}
public slots:
    int restore(QList<Person*> *people);
private:
    void printValues(const QString &area) const;
    QList<Person*> *people_;
    int rows_;
    int columns_;
signals:
    void oUpdated(int, int, Person*);
};

class SortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    SortFilterProxyModel(QObject *parent = nullptr);
    QList<Person*>* people() {return people_;}
    void people(QList<Person*>* p) {people_ = p;}
private:
    QList<Person*> *people_;
};
