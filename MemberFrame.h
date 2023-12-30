#pragma once
#include <QTabWidget>
#include <QLayout>
#include "MemberFrame.h"
#include "TableView.h"

class MemberFrame : public QTabWidget {
Q_OBJECT
public:
    MemberFrame(QWidget *parent = 0);
    TableView *memberView_;
signals:
    void oApply();
};
