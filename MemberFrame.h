#pragma once
#include <QTabWidget>
#include <QLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include "TableView.h"

class MemberFrame : public QTabWidget {
Q_OBJECT
public:
    MemberFrame(QWidget *parent = 0);
    TableView *memberView_;
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent *);
private:
    QRect pos_;
signals:
    void oApply();
    void oSearchChanged(const QString&);
};
