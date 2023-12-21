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
    Person *movingPerson_;
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
    virtual bool event(QEvent *event);
private slots:
    void importDatabase();
    void exportDatabase();
    void setupColors();
    void assignPeopleToRooms();
signals:
    void eAssignColors();
};
