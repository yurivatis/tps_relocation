#pragma once
#include <QTabWidget>
#include <QLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include "TableView.h"

class RoomFrame : public QTabWidget {
Q_OBJECT
public:
    RoomFrame(QWidget *parent = 0);
    TableView *roomView_;
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent *);
private:
    QRect pos_;
signals:
    void oSave();
};
