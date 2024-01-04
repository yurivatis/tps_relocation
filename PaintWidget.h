#pragma once
#include <QWidget>
#include <QPaintEvent>
#include "MainWindow.h"

class MainWindow;

class PaintWidget : public QWidget {
    Q_OBJECT
public:
    PaintWidget(QWidget* parent = nullptr);
public slots:
    void makeScreenshot();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual bool event(QEvent *event);
private:
    MainWindow *parent_;
};
