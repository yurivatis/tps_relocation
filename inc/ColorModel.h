#pragma once
#include <QStandardItemModel>
#include <QVariant>
#include <QList>
#include "PushButtonDelegate.h"
#include "ComboboxDelgate.h"

typedef struct modelValue {
    int row;
    int col;
    QString value;
} ModelValue;

class ColorModel : public QStandardItemModel {
Q_OBJECT
public:
    ColorModel(int, int, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override;
public slots:
    void setComboBox(const QModelIndex&, const QString);
private:
    int rows_;
    int columns_;
    QList<ModelValue> list_;
};
