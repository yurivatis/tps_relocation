#pragma once
#include <QString>
#include <QMainWindow>
#include <QPaintEvent>
#include <QList>

#include "Room.h"
#include "ColorModel.h"
#include "ColorView.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QApplication *app, QWidget *parent = 0);
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    void createMenus();
    void drawFirstFloor();
    void addRooms();
    QList<Room*>rooms_;
    ColorModel *colorModel_;
    QStandardItemModel * model_;
    ColorView *colorView_;
    ComboBoxDelegate * departmentDelegate_;
    ComboBoxDelegate * teamDelegate_;
    ComboBoxDelegate * componentDelegate_;
    PushButtonDelegate * colorDelegate_;
    PushButtonDelegate *addDelegate_;
    PushButtonDelegate * removeDelegate_;
    QMenu *customizeMenu_;
    QMenu *dbMenu_;
    QAction *importDb_;
    QAction *colors_;

private slots:
    void importDatabase();
    void setupColors();
};
