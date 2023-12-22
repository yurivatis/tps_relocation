#pragma once
#include <QTabWidget>
#include <QLayout>
#include "ColorView.h"
class ColorFrame : public QTabWidget {
Q_OBJECT
public:
    ColorFrame(QWidget *parent = 0);
    ColorView *colorView_;
signals:
    void oRemoveAll();
    void oAddRow();
    void oSave();
    void oClose();
};
