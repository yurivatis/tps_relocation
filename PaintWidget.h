#pragma once
#include <QWidget>
#include <QPaintEvent>
#include "MainWindow.h"

class MainWindow;

class PaintWidget : public QWidget {
    Q_OBJECT
public:
    PaintWidget(QWidget* parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    MainWindow *parent_;
};
