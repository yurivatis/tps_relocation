#pragma once
#include <QStandardItemModel>
#include <QVariant>
#include <QList>

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
    QList<ModelValue> *modelValues() {return &list_;}
public slots:
    void setComboBox(QModelIndex, QString);
    void setColor(QModelIndex&, QColor);
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

typedef struct rowData {

    rowData() = default;
    rowData( const QString &department, const QString &team, const QString &component, const QColor &color) :
        department_( department ), team_( team ), component_( component ), color_( color ) {};

    QString department_;
    QString team_;
    QString component_;
    QColor color_;
} RowData;

class ColorModel2 : public QAbstractTableModel  {
    Q_OBJECT
public:
    ColorModel2(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &data, int role = Qt::DisplayRole) override;

    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;

    bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() ) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void loadDefault();
    void addRow();
    void removeAll();
    void removeRow( int row );
    void save();
    int restore();

private:
    QVector< RowData > data_;

    QString firstTeamOfDepartment( const QString &department ) const;

signals:
    void oUpdated();
};
