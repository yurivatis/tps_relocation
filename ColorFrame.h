#pragma once
#include <QTabWidget>
#include <QLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include "TableView.h"

class ColorFrame : public QTabWidget {
Q_OBJECT
public:
    ColorFrame(QWidget *parent = 0);
    TableView *colorView_;
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent *);
private:
    QRect pos_;
signals:
    void oLoadDefault();
    void oRestore();
    void oRemoveAll();
    void oAddRow();
    void oSave();
    void oClose();
};
