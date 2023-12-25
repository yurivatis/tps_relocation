#pragma once
#include <QTabWidget>
#include <QLayout>
#include "TableView.h"
class ColorFrame : public QTabWidget {
Q_OBJECT
public:
    ColorFrame(QWidget *parent = 0);
    TableView *colorView_;
signals:
    void oRestore();
    void oRemoveAll();
    void oAddRow();
    void oSave();
    void oClose();
};
