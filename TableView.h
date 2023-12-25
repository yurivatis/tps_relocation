#pragma once
#include <QTableView>
#include "ComboboxDelgate.h"

class TableView : public QTableView {
Q_OBJECT
public:
    TableView(QWidget *parent = 0);
};
