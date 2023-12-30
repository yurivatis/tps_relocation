#pragma once
#include <QString>
#include <QMainWindow>
#include <QPaintEvent>
#include <QList>

#include "Room.h"
#include "ColorModel.h"
#include "ColorFrame.h"
#include "RemoveButtonDelegate.h"
#include "LineEditDelegate.h"
#include "MemberModel.h"
#include "MemberFrame.h"
#include "HelpWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QApplication *app, QWidget *parent = 0);
    virtual ~MainWindow();
    QList<Person*>* people() {return &people_;}
protected:
    virtual void paintEvent(QPaintEvent *);
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
    MemberFrame *memberFrame_;
    ComboBoxDelegate * departmentDelegate_;
    ComboBoxDelegate * teamDelegate_;
    ComboBoxDelegate * componentDelegate_;
    ColorButtonDelegate * colorDelegate_;
    RemoveButtonDelegate * removeDelegate_;
    LineEditDelegate *memberNameDelegate_;
    LineEditDelegate *memberRoomDelegate_;
    Person *movingPerson_;
    HelpWidget *helpWidget_;
    bool unstored_;
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
    virtual bool event(QEvent *event);
private slots:
    void importDatabase();
    void exportDatabase();
    void exportCsv();
    void toInitState();
    void setupColors();
    void assignPeopleToRooms();
    void makeScreenshot();
    void updateMates();
signals:
    void eAssignColors();
};
