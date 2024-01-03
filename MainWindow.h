#pragma once
#include <QString>
#include <QMainWindow>
#include <QPaintEvent>
#include <QList>
#include <QSortFilterProxyModel>

#include "Room.h"
#include "ColorModel.h"
#include "ColorFrame.h"
#include "MemberModel.h"
#include "MemberFrame.h"
#include "HelpWidget.h"
#include "PaintWidget.h"

class PaintWidget;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QApplication *app, QWidget *parent = 0);
    virtual ~MainWindow();
    QList<Person*>* people() {return &people_;}
    QList<Room*>* rooms() {return &rooms_;}
    Person *moving() {return movingPerson_;}
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
    virtual bool event(QEvent *event);
//    virtual void paintEvent(QPaintEvent *);
private:
    void createMenus();
    void addRooms();
    void addPeople();
    void printPeople();
    void printRooms();
    void redrawMates(Room *r);
    QList<Room*>rooms_;
    QList<Person*>people_;
    ColorModel *colorModel_;
    ColorFrame *colorFrame_;
    MemberModel *memberModel_;
    SortFilterProxyModel *proxyMemberModel_;
    MemberFrame *memberFrame_;
    Person *movingPerson_;
    HelpWidget *helpWidget_;
    PaintWidget *paintWidget_;
    bool unstored_;
private slots:
    void importDatabase();
    void exportDatabase();
    void exportCsv();
    void toInitState();
    void assignPeopleToRooms();
    void makeScreenshot();
    void updateMates();
signals:
    void eAssignColors();
};
