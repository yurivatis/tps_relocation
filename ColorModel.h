#pragma once
#include <QAbstractTableModel>
#include <QVariant>
#include <QList>
#include <QColor>

typedef struct modelValue {
    int row;
    int col;
    QString value;
    QColor color;
} ModelValue;

typedef QMutableListIterator<ModelValue> ModelListIterator;


typedef struct rowData {

    rowData() = default;
    rowData( const QString &department, const QString &team, const QString &component, const QColor &color) :
        department_( department ), team_( team ), component_( component ), color_( color ) {};

    QString department_;
    QString team_;
    QString component_;
    QColor color_;
} RowData;

class ColorModel : public QAbstractTableModel {
    Q_OBJECT
public:
    ColorModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &data, int role = Qt::DisplayRole) override;

    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
    bool removeRows( int row, int count, const QModelIndex &parent ) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
public slots:
    void addRow();
    void loadDefault();
    void removeRow(int);
    void removeAll();
    void save();
    int  restore();
private:
    QVector< RowData > data_;
    QString firstTeamOfDepartment( const QString &department ) const;

signals:
    void oUpdated();
};
