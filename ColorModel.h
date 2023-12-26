#pragma once
#include <QStandardItemModel>
#include <QVariant>
#include <QList>
#include "ColorButtonDelegate.h"
#include "ComboboxDelgate.h"

typedef struct modelValue {
    int row;
    int col;
    QString value;
    QColor color;
} ModelValue;

typedef QMutableListIterator<ModelValue> ModelListIterator;


class ColorModel : public QStandardItemModel {
Q_OBJECT
public:
    ColorModel(int, int, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
public slots:
    void setComboBox(const QModelIndex&, const QString);
    void setColor(const QModelIndex, const QColor);
    void addRow();
    void loadDefault();
    void removeRow(int);
    void removeAll();
    void save();
    int restore();
private:
    void fillColorRow(int, QString, QString, QString, QColor);
    void printValues(const QString &area) const;
    int rows_;
    int columns_;
    QList<ModelValue> list_;
signals:
    void oUpdated();
};
