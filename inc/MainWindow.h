#pragma once
#include <QString>
#include <QMainWindow>
#include <QPaintEvent>
#include <QList>

#include "Room.h"
#include "ColorModel.h"
#include "ColorFrame.h"
#include "RemoveButtonDelegate.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QApplication *app, QWidget *parent = 0);
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    void createMenus();
    void addRooms();
    void addPeople();
    void printPeople();
    void printRooms();
    void redrawMates(Room *r);
    void placePersonInRoom(int oldRoom, int newRoom, Person *p);
    void placePersonInRoom(Room *oldRoom, Room *newRoom, Person *p);
    QList<Room*>rooms_;
    QList<Person*>people_;
    ColorModel *colorModel_;
    QStandardItemModel * model_;
    ColorFrame *colorFrame_;
    ComboBoxDelegate * departmentDelegate_;
    ComboBoxDelegate * teamDelegate_;
    ComboBoxDelegate * componentDelegate_;
    ColorButtonDelegate * colorDelegate_;
    RemoveButtonDelegate * removeDelegate_;
    QMenu *customizeMenu_;
    QMenu *dbMenu_;
    QAction *importDb_;
    QAction *colors_;
    Person *movingPerson_;
protected:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
private slots:
    void importDatabase();
    void setupColors();
    void assignPeopleToRooms();
signals:
    void eAssignColors();
};
