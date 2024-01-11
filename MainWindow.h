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
#include "RoomModel.h"
#include "RoomFrame.h"
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
    void moving(Person *p) {movingPerson_ = p;}
    void unstored(bool u) {unstored_ = u;}
public slots:
    void updateMates();
    void updateCapacities();
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    void createMenus();
    void addRooms();
    void addPeople();
    void printPeople();
    void printRooms();
    void redrawMates(Room *r);
    void showFrame(QWidget *);
    QList<Room*>rooms_;
    QList<Person*>people_;
    ColorModel *colorModel_;
    ColorFrame *colorFrame_;
    MemberModel *memberModel_;
    SortFilterProxyModel *proxyMemberModel_;
    SortRoomProxyModel *proxyRoomModel_;
    MemberFrame *memberFrame_;
    RoomModel *roomModel_;
    RoomFrame *roomFrame_;
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
    void showHelpWidget();
    void showColorFrame();
    void showMemberFrame();
    void showRoomFrame();
signals:
    void eAssignColors();
};
