#pragma once
#include <QTableView>

class TableView : public QTableView {
Q_OBJECT
public:
    TableView(QWidget *parent = 0);

private slots:
    void cellClicked( const QModelIndex &index );
};

