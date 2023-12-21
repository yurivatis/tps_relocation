#pragma once
#include <QTableView>
#include "ComboboxDelgate.h"

class ColorView : public QTableView {
Q_OBJECT
public:
    ColorView(QWidget *parent = 0);
};
