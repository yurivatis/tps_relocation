#pragma once
#include <QStandardItemModel>
#include <QVariant>
#include <QList>
#include "PushButtonDelegate.h"
#include "ComboboxDelgate.h"
class ColorModel : public QStandardItemModel {
Q_OBJECT
public:
    ColorModel(int, int, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    int rows_;
    int columns_;
};
