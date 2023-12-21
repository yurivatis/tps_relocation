#include "MainWindow.h"
#include "constants.h"
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QPolygonF>
#include <QPoint>
#include <QPen>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
#include <math.h>
#include <QPainterPath>
#include <QMatrix>
#include <QPixmap>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScrollArea>
#include "SQLite.h"

MainWindow::MainWindow(QApplication *app, QWidget *parent): QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
//     QScrollArea *scrollArea = new QScrollArea(this);
//     scrollArea->setWidget(mainWidget);
//     scrollArea->setWidgetResizable(true);
//    setCentralWidget(scrollArea);
    setCentralWidget(mainWidget);
    createMenus();
    colorModel_ = new ColorModel(SqlInterface::getInstance()->colorEntries() , (int)Column::TOTAL_COLUMNS);
    colorModel_->restore();

    colorFrame_ = new ColorFrame();
    colorFrame_->setWindowModality(Qt::ApplicationModal);
    colorFrame_->colorView_->setModel(colorModel_);
    colorFrame_->setWindowTitle(QObject::tr("Customize colors"));

    departmentDelegate_ = new ComboBoxDelegate();
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::DEPARTMENT, departmentDelegate_);

    teamDelegate_ = new ComboBoxDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::TEAM, teamDelegate_);

    componentDelegate_ = new ComboBoxDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::COMPONENT, componentDelegate_);

    colorDelegate_ = new ColorButtonDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::COLOR, colorDelegate_);

    removeDelegate_ = new RemoveButtonDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::REMOVE, removeDelegate_);

    addRooms();
    addPeople();
    assignPeopleToRooms();

//    setMinimumSize(1500, 1000);
    QObject::connect(departmentDelegate_, SIGNAL(oComboText(const QString)), teamDelegate_, SLOT(getDepartment(const QString)));
    QObject::connect(teamDelegate_, SIGNAL(oComboText(const QString)), componentDelegate_, SLOT(getTeam(const QString)));
    QObject::connect(departmentDelegate_, SIGNAL(oComboChanged(const QModelIndex &, const QString)), colorModel_, SLOT(setComboBox(const QModelIndex&, const QString)));
    QObject::connect(teamDelegate_, SIGNAL(oComboChanged(const QModelIndex &, const QString)), colorModel_, SLOT(setComboBox(const QModelIndex&, const QString)));
    QObject::connect(componentDelegate_, SIGNAL(oComboChanged(const QModelIndex &, const QString)), colorModel_, SLOT(setComboBox(const QModelIndex&, const QString)));
    QObject::connect(colorDelegate_, SIGNAL(oColorChanged(const QModelIndex, const QColor)), colorModel_, SLOT(setColor(const QModelIndex, const QColor)));
    QObject::connect(colorFrame_, SIGNAL(oAddRow()), colorModel_, SLOT(addRow()));
    QObject::connect(removeDelegate_, SIGNAL(oRemoveRow(int)), colorModel_, SLOT(removeRow(int)));
    QObject::connect(colorFrame_, SIGNAL(oSave()), colorModel_, SLOT(save()));
    QObject::connect(colorModel_, SIGNAL(oUpdated()), this, SLOT(assignPeopleToRooms()));
    
    QString message = tr("Detailed plan of Hacon's 1st floor");
    statusBar()->showMessage(message);
//     printRooms();
}


void MainWindow::createMenus()
{
    dbMenu_ = menuBar()->addMenu(tr("&Database"));
    customizeMenu_ = menuBar()->addMenu(tr("&Customize"));

    importDb_ = new QAction(tr("&Import db"), this);
    importDb_->setShortcut(tr("Ctrl+i"));
    dbMenu_->addAction(importDb_);
    QObject::connect(importDb_, SIGNAL(triggered()), this, SLOT(importDatabase()));

    colors_ = new QAction(tr("&Colors"), this);
    customizeMenu_->addAction(colors_);
    QObject::connect(colors_, SIGNAL(triggered()), this, SLOT(setupColors()));
}


void MainWindow::addRooms()
{
    rooms_.append(new Room(105, 4, {  50, 900,   50, 800, 130, 800, 130, 900}));
    rooms_.append(new Room(106, 4, { 130, 900,  130, 800, 210, 800, 210, 900}));
    rooms_.append(new Room(108, 3, { 210, 900,  210, 800, 260, 800, 260, 900}));
    rooms_.append(new Room(109, 3, { 260, 900,  260, 800, 310, 800, 310, 900}));
    rooms_.append(new Room(110, 3, { 310, 900,  310, 800, 360, 800, 360, 900}));
    rooms_.append(new Room(111, 3, { 360, 900,  360, 800, 410, 800, 410, 900}));
    rooms_.append(new Room(113, 3, { 410, 900,  410, 800, 460, 800, 460, 900}));
    rooms_.append(new Room(114, 3, { 460, 900,  460, 800, 510, 800, 510, 900}));
    rooms_.append(new Room(115, 3, { 510, 900,  510, 800, 560, 800, 560, 900}));
    rooms_.append(new Room(116, 3, { 560, 900,  560, 800, 610, 800, 610, 900}));
    rooms_.append(new Room(159, 3, { 610, 900,  610, 800, 660, 800, 660, 900}));
    rooms_.append(new Room(160, 3, { 660, 900,  660, 800, 710, 800, 710, 900}));
    rooms_.append(new Room(161, 3, { 710, 900,  710, 800, 760, 800, 760, 900}));
    rooms_.append(new Room(162, 3, { 760, 900,  760, 800, 810, 800, 810, 900}));
    rooms_.append(new Room(163, 3, { 810, 900,  810, 800, 840, 800, 900, 860, 900, 900}));
    rooms_.append(new Room(  0, 0, { 900, 900,  900, 960, 960, 960}, Orientation::CENTER, true));
    rooms_.append(new Room(176, 4, { 960, 960,  960, 860, 1060, 860, 1060, 960}));
    rooms_.append(new Room(175, 2, {1060, 960, 1060, 900, 1120, 900, 1120, 960}));
    rooms_.append(new Room(173, 2, {1120, 960, 1120, 900, 1180, 900, 1180, 960}));
    rooms_.append(new Room(174, 0, {1070, 900, 1070, 860, 1170, 860, 1170, 900}));
    rooms_.append(new Room(172, 8, {1180, 960, 1180, 860, 1300, 860, 1300, 960}));
    rooms_.append(new Room(171, 8, {1300, 960, 1300, 860, 1400, 860, 1400, 960}));
    rooms_.append(new Room(  0, 0, {1350, 860, 1350, 780, 1180, 780}, Orientation::CENTER, true));
    rooms_.append(new Room(169, 2, {1120, 840, 1120, 780, 1180, 780, 1180, 840}, Orientation::UP));
    rooms_.append(new Room(168, 3, {1060, 840, 1060, 780, 1120, 780, 1120, 840}, Orientation::UP));
    rooms_.append(new Room(166, 4, { 980, 840,  980, 780, 1060, 780, 1060, 840}, Orientation::UP));
    rooms_.append(new Room(166, 4, { 980, 840,  980, 780, 1060, 780, 1060, 840}, Orientation::UP));
    rooms_.append(new Room(  0, 0, { 980, 840,  930, 840,  930, 710,  980, 710, 980, 780}, Orientation::CENTER, true));
    rooms_.append(new Room(164, 0, { 860, 770,  860, 710,  930, 710,  930, 840}, Orientation::CENTER));
    rooms_.append(new Room(158, 4, { 880, 710,  880, 610,  980, 610,  980, 710}, Orientation::UP));
    rooms_.append(new Room(157, 2, { 810, 680,  810, 610,  880, 610,  880, 680}, Orientation::UP));
    rooms_.append(new Room(156, 2, { 740, 680,  740, 610,  810, 610,  810, 680}, Orientation::UP));
    rooms_.append(new Room(155, 4, { 670, 680,  670, 610,  740, 610,  740, 680}, Orientation::UP));
    rooms_.append(new Room(154, 4, { 610, 740,  610, 610,  670, 610,  670, 740}, Orientation::UP));
    rooms_.append(new Room(153, 0, { 610, 770,  610, 740,  670, 740,  670, 770}, Orientation::CENTER));
    rooms_.append(new Room(165, 0, { 700, 770,  700, 710,  830, 710,  830, 770}, Orientation::CENTER));

}


void MainWindow::addPeople()
{
    SqlInterface::getInstance()->people(people_, this);
}


void MainWindow::assignPeopleToRooms()
{
    foreach(Person *p, people_) {
        p->color(SqlInterface::getInstance()->readColor(p->department(), p->team(), p->component(), Qt::gray)); // set colors
        int curRoom = p->room();
        foreach(Room *r, rooms_) { // assign to rooms
            if(r->nr() == curRoom) {
                r->addPerson(p);
                placePersonInRoom(nullptr, r, p);
                break;
            }
        }
    }
    update();
}


void MainWindow::placePersonInRoom(int oldRoom, int newRoom, Person *p)
{
    Room *oldR = nullptr, *newR = nullptr;
    bool found_old = false, found_new = false;
    foreach(Room *r, rooms_) {
        if(r->nr() == oldRoom) {
            oldR = r;
            found_old = true;
        }
        if(r->nr() == newRoom) {
            newR = r;
            found_new = true;
        }
        if(found_new && found_old) {
            break;
        }
    }
    placePersonInRoom(oldR, newR, p);
}


void MainWindow::placePersonInRoom(Room *oldRoom, Room *newRoom, Person *p)
{
    if(oldRoom && oldRoom->nr() != 0) {
        oldRoom->removePerson(p);
        p->room(0);
        redrawMates(oldRoom);
    }
    if(newRoom && newRoom->nr() != 0) {
        newRoom->addPerson(p);
        p->room(newRoom->nr());
        redrawMates(newRoom);
    }
}


void MainWindow::redrawMates( Room* r)
{
    int nr = r->people_.size();
    if(nr == 0) {
        return;
    }
    QPolygonF roomPlg = r->coordinates();
    float stepX = (roomPlg.at(roomPlg.size() - 1).x() - roomPlg.at(0).x()) / nr;
    float stepY = (roomPlg.at(roomPlg.size() - 1).y() - roomPlg.at(0).y()) / nr;
    for(int i = 0; i < nr; i++) {
        QPolygonF personPlg;
        QPoint p0, p1, p2, p3;
        p0.setX(roomPlg.at(0).x() + i * stepX);
        p0.setY(roomPlg.at(0).y() + i * stepY);
        personPlg << p0;
        p1.setX(roomPlg.at(1).x() + i * stepX);
        p1.setY(roomPlg.at(1).y() + i * stepY);
        personPlg << p1;
        p2.setX(roomPlg.at(1).x() + (i+1) * stepX);
        p2.setY(roomPlg.at(1).y() + (i+1) * stepY);
        personPlg << p2;
        p3.setX(roomPlg.at(0).x() + (i+1) * stepX);
        p3.setY(roomPlg.at(0).y() + (i+1) * stepY);
        personPlg << p3;
        r->people_.at(i)->coordinates(personPlg);
    }
}


void MainWindow::printPeople()
{
    qDebug() << "============== number or people: " << people_.size() << "==============";
    int i = 0;
    foreach(Person *p, people_) {
        i++;
        qDebug() << i << ": " << p->surname() << " | " << p->name() << " | " << p->location() << " | " << p->department() << " | " << p->team() << " | " << p->component() << " | " << p->room();
    }
}


void MainWindow::printRooms()
{
    qDebug() << "============== number or rooms: " << rooms_.size() << "==============";
    foreach(Room *r, rooms_) {
        if(r->dummy() || r->nr() == 0) {
            continue;
        }
        qDebug() << "current: " << r->nr();
        foreach(Person *p, r->people_) {
            qDebug() << ": " << p->surname() << " | " << p->name() ;
        }
    }
}


void MainWindow::importDatabase()
{
    bool res = false;
    QMessageBox msgBox;
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open csv file"), "./", tr("csv files (*.csv)"));
    if(fileName.length() > 0) {
        res = sqlInteface->import(fileName);
        if(res) {
            msgBox.setText(QString("Successfully imported from %1").arg(fileName));
            msgBox.setIcon(QMessageBox::Information);
        } else {
            msgBox.setText(QString("Import from %1 failed").arg(fileName));
            msgBox.setIcon(QMessageBox::Warning);
        }
        msgBox.exec();
    }
}


void MainWindow::setupColors()
{
    colorFrame_->show();
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    QPalette pal = QPalette();
    QFont fnt("Helvetica", 7, QFont::Normal);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    for(int i = 0; i < rooms_.size(); ++i) {
        int room_nr = rooms_.at(i)->nr();
        for(int j = 0; j < rooms_.at(i)->people_.size(); j++) {
            Person *p = rooms_.at(i)->people_.at(j);
            QColor color = p->color();
            pen.setWidthF(0.5);
            painter.setPen(pen);
            QPainterPath path;
            if(p == movingPerson_) {
                path.addPolygon(p->tmpcoordinates());
                color.setAlpha(127);
            } else {
                path.addPolygon(p->coordinates());
            }
            brush.setColor(color);
            if(p->role() == "Team Lead") {
                brush.setStyle(Qt::Dense2Pattern);
            } else {
                brush.setStyle(Qt::SolidPattern);
            }
            painter.fillPath(path, brush);
            painter.drawPolygon(p->coordinates());
            if(room_nr != 0 && !rooms_.at(i)->dummy() && p) {
                painter.save();
                painter.translate(p->coordinates().at(0).x(), p->coordinates().at(0).y());
                painter.rotate(-90);
                painter.drawText(6, 12, p->displayName());
                painter.restore();
            }
        }
        pen.setWidthF(2);
        painter.setPen(pen);
        if(!rooms_.at(i)->dummy()) {
            painter.drawPolygon(rooms_.at(i)->coordinates());
            painter.drawText(rooms_.at(i)->getCenter(), QString::number(rooms_.at(i)->nr()));
        } else {
            painter.drawPolyline(rooms_.at(i)->coordinates());
        }
    }
}


void MainWindow::mousePressEvent(QMouseEvent* mouseEvent)
{
    movingPerson_ = nullptr;
    QPointF f(mouseEvent->x(), mouseEvent->y());
    if(mouseEvent->button() != Qt::LeftButton) {
        return;
    }
    foreach(Person*p, people_) {
        if(p->coordinates().containsPoint(f, Qt::WindingFill)) {
            movingPerson_ = p;
            p->offset(f.x(), f.y());
            break;
        }
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    int oldRoom = 0;
    int newRoom = 0;
    if(mouseEvent->button() != Qt::LeftButton || movingPerson_ == nullptr) {
        return;
    }
    QPointF f(mouseEvent->x(), mouseEvent->y());
    foreach(Room *r, rooms_) {
        if(r->coordinates().containsPoint(f, Qt::WindingFill)) {
            newRoom = r->nr();
            oldRoom = movingPerson_->room();
            if(oldRoom == r->nr()) {
                break;
            }
            r->addPerson(movingPerson_);
            movingPerson_->room(r->nr());
            redrawMates(r);
            break;
        }
    }
    if(oldRoom != 0 && oldRoom != newRoom) {
        foreach(Room *r, rooms_) {
            if(r->nr() == oldRoom) {
                r->removePerson(movingPerson_);
                redrawMates(r);
                break;
            }
        }
    }
    movingPerson_->clear();
    movingPerson_ = nullptr;
    update();
}


void MainWindow::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    if(movingPerson_ == nullptr) {
        return;
    }
    movingPerson_->moveTo(mouseEvent->x(), mouseEvent->y());
    update();
}

