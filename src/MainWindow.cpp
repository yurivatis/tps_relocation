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
#include <QToolTip>
#include <QLabel>
#include "SQLite.h"

MainWindow::MainWindow(QApplication *, QWidget *parent): QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
//     QScrollArea *scrollArea = new QScrollArea(this);
//     scrollArea->setWidget(mainWidget);
//     scrollArea->setWidgetResizable(true);
//    setCentralWidget(scrollArea);
    setCentralWidget(mainWidget);
    QPixmap pixmap("./hacon.png");
    QLabel *icon = new QLabel("", this);
    icon->setPixmap(QString::fromUtf8(":/images/hacon.png"));
    icon->setGeometry(900, 50, 200, 200);
    createMenus();
    colorModel_ = new ColorModel(SqlInterface::getInstance()->colorEntries() , (int)Column::TOTAL_COLUMNS);
    colorModel_->restore();

    colorFrame_ = new ColorFrame();
    colorFrame_->setWindowModality(Qt::ApplicationModal);
    colorFrame_->colorView_->setModel(colorModel_);
    colorFrame_->setWindowTitle(QObject::tr("Customize colors"));
    

    departmentDelegate_ = new ComboBoxDelegate(colorFrame_->colorView_);
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
    QMenu *dbMenu = menuBar()->addMenu(tr("&Database"));
    QMenu *customizeMenu = menuBar()->addMenu(tr("&Customize"));

    QAction *importDb = new QAction(tr("&Import from db"), this);
    importDb->setShortcut(tr("Ctrl+i"));
    dbMenu->addAction(importDb);
    QObject::connect(importDb, SIGNAL(triggered()), this, SLOT(importDatabase()));

    QAction *exportDb = new QAction(tr("&Export to db"), this);
    exportDb->setShortcut(tr("Ctrl+e"));
    dbMenu->addAction(exportDb);
    QObject::connect(exportDb, SIGNAL(triggered()), this, SLOT(exportDatabase()));

    
    QAction * colors = new QAction(tr("&Colors"), this);
    customizeMenu->addAction(colors);
    QObject::connect(colors, SIGNAL(triggered()), this, SLOT(setupColors()));
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
    rooms_.append(new Room(  0, 0, { 900, 900,  900, 960, 960, 960}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(176, 4, { 960, 960,  960, 860, 1060, 860, 1060, 960}));
    rooms_.append(new Room(175, 2, {1060, 960, 1060, 900, 1120, 900, 1120, 960}));
    rooms_.append(new Room(173, 2, {1120, 960, 1120, 900, 1180, 900, 1180, 960}));
    rooms_.append(new Room(174, 0, {1070, 900, 1070, 860, 1170, 860, 1170, 900}));
    rooms_.append(new Room(172, 8, {1180, 960, 1180, 860, 1300, 860, 1300, 960}));
    rooms_.append(new Room(171, 8, {1300, 960, 1300, 860, 1400, 860, 1400, 960}));
    rooms_.append(new Room(  0, 0, {1350, 860, 1350, 760, 1310, 760}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(  0, 0, {1190, 840, 1190, 760, 1310, 760, 1310, 840}, Orientation::CENTER));
    rooms_.append(new Room(169, 2, {1130, 840, 1130, 760, 1190, 760, 1190, 840}, Orientation::UP));
    rooms_.append(new Room(168, 3, {1060, 840, 1060, 760, 1130, 760, 1130, 840}, Orientation::UP));
    rooms_.append(new Room(166, 4, { 980, 840,  980, 760, 1060, 760, 1060, 840}, Orientation::UP));
    rooms_.append(new Room(  0, 0, { 930, 840,  930, 700,  980, 700,  980, 840}, Orientation::CENTER, -90));
    rooms_.append(new Room(164, 0, { 860, 770,  860, 700,  930, 700,  930, 840}, Orientation::CENTER));
    rooms_.append(new Room(158, 4, { 880, 700,  880, 610,  980, 610,  980, 700}, Orientation::UP));
    rooms_.append(new Room(157, 2, { 810, 680,  810, 610,  880, 610,  880, 680}, Orientation::UP));
    rooms_.append(new Room(156, 2, { 740, 680,  740, 610,  810, 610,  810, 680}, Orientation::UP));
    rooms_.append(new Room(155, 4, { 670, 680,  670, 610,  740, 610,  740, 680}, Orientation::UP));
    rooms_.append(new Room(154, 4, { 610, 740,  610, 610,  670, 610,  670, 740}, Orientation::UP));
    rooms_.append(new Room(153, 0, { 610, 780,  610, 740,  670, 740,  670, 780}, Orientation::CENTER));
    rooms_.append(new Room(165, 0, { 695, 780,  695, 700,  835, 700,  835, 780}, Orientation::CENTER));
    rooms_.append(new Room(117, 0, { 505, 780,  505, 710,  610, 710,  610, 780}, Orientation::CENTER));
    rooms_.append(new Room(118, 0, { 505, 710,  505, 620,  610, 620,  610, 710}, Orientation::CENTER));
    rooms_.append(new Room(0,   0, {  50, 800,   50, 780}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(0,   0, {  50, 780,   50, 700,  130, 700,  130, 780}));
    rooms_.append(new Room(0,   0, { 130, 780,  130, 740,  150, 740,  150, 780, 130, 780}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(0,   0, { 130, 740,  130, 700,  150, 700,  150, 740, 130, 740}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(104, 5, {  50, 700,   50, 590,  130, 590,  130, 700}, Orientation::UP));
    rooms_.append(new Room(103, 6, { 130, 680,  130, 590,  210, 590,  210, 680}, Orientation::UP));
    rooms_.append(new Room(101, 4, { 210, 680,  210, 610,  310, 610,  310, 680}, Orientation::UP));
    rooms_.append(new Room(107, 0, { 170, 780,  170, 700,  240, 700,  240, 780}, Orientation::CENTER));
    rooms_.append(new Room(102, 0, { 240, 780,  240, 700,  310, 700,  310, 780}, Orientation::CENTER));
    rooms_.append(new Room(  0, 0, { 310, 780,  310, 700,  350, 700,  350, 780}, Orientation::CENTER));
    rooms_.append(new Room(112, 0, { 350, 780,  350, 700,  430, 700,  430, 780}, Orientation::CENTER));
    rooms_.append(new Room(  0, 0, { 430, 780,  430, 700,  470, 700,  470, 780, 430, 780}, Orientation::CENTER, -90, true));

    rooms_.append(new Room(119, 4, { 490, 565,  587, 530,  610, 610,  610, 620, 505, 620}, Orientation::RIGHT, -18));
    rooms_.append(new Room(120, 4, { 475, 510,  572, 475,  587, 530,  490, 565}, Orientation::RIGHT, -18));

    rooms_.append(new Room(121, 0, { 355, 550,  444, 520,  462, 595,  345, 595}, Orientation::CENTER, 0));

    rooms_.append(new Room(149, 2, { 448, 435,  551, 400,  572, 475,  470, 511}, Orientation::RIGHT, -18));
    rooms_.append(new Room(148, 2, { 426, 359,  529, 324,  551, 400,  448, 435}, Orientation::RIGHT, -18));
    rooms_.append(new Room(147, 3, { 415, 321,  518, 286,  529, 324,  426, 359}, Orientation::RIGHT, -18));
    rooms_.append(new Room(146, 3, { 404, 283,  507, 248,  518, 286,  415, 321}, Orientation::RIGHT, -18));
    rooms_.append(new Room(145, 3, { 393, 245,  496, 210,  507, 248,  404, 283}, Orientation::RIGHT, -18));
    rooms_.append(new Room(144, 3, { 382, 207,  485, 172,  496, 210,  393, 245}, Orientation::RIGHT, -18));
    rooms_.append(new Room(143, 0, { 349,  93,  452,  58,  485, 172,  382, 207}, Orientation::RIGHT, -18));
//    rooms_.append(new Room(121, 0, { 430, 780,  430, 700,  470, 700,  470, 780, 430, 780}, Orientation::CENTER, -90, true));
    
    rooms_.append(new Room(  0, 0, { 181, 590,  181, 486}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(131, 2, { 170, 448,  273, 413,  284, 451,  181, 486}, Orientation::LEFT, -18));
    rooms_.append(new Room(132, 2, { 159, 410,  262, 375,  273, 413,  170, 448}, Orientation::LEFT, -18));
    rooms_.append(new Room(133, 3, { 148, 372,  251, 337,  262, 375,  159, 410}, Orientation::LEFT, -18));
    rooms_.append(new Room(134, 3, { 137, 334,  240, 299,  251, 337,  148, 372}, Orientation::LEFT, -18));
    rooms_.append(new Room(135, 3, { 126, 296,  229, 261,  240, 299,  137, 334}, Orientation::LEFT, -18));
    rooms_.append(new Room(136, 3, { 115, 258,  218, 223,  229, 261,  126, 296}, Orientation::LEFT, -18));
    rooms_.append(new Room(137, 3, { 104, 220,  207, 185,  218, 223,  115, 258}, Orientation::LEFT, -18));
    rooms_.append(new Room(138, 4, {  88, 163,  191, 128,  207, 185,  104, 220}, Orientation::LEFT, -18));
    rooms_.append(new Room(139, 4, {  72, 106,  175,  71,  191, 128,   88, 163}, Orientation::LEFT, -18));
    
    rooms_.append(new Room(151, 0, {  270, 302,  368, 268,  408, 406,  308, 438}, Orientation::CENTER, -18));
    rooms_.append(new Room(140, 0, {  202,  61,  301,  28,  333, 136,  232, 168}, Orientation::CENTER, -18));
}


void MainWindow::addPeople()
{
    SqlInterface::getInstance()->people(people_);
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


void MainWindow::exportDatabase()
{
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QMessageBox msgBox;
    if(sqlInteface->exportTo(people_) == false) {
        msgBox.setText(QString("Successfully exported"));
        msgBox.setIcon(QMessageBox::Information);
    } else {
        msgBox.setText(QString("Export failed"));
        msgBox.setIcon(QMessageBox::Warning);
    }
    msgBox.exec();
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
            rooms_.clear();
            people_.clear();
            addRooms();
            addPeople();
            assignPeopleToRooms();

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
            if(p->isLead()) {
                brush.setStyle(Qt::Dense2Pattern);
            } else {
                brush.setStyle(Qt::SolidPattern);
            }
            painter.fillPath(path, brush);
            painter.drawPolygon(p->coordinates());
            if(room_nr != 0 && !rooms_.at(i)->dummy() && p) {
                painter.save();
                painter.translate(p->coordinates().at(0).x(), p->coordinates().at(0).y());
                painter.rotate(rooms_.at(i)->rotation());
                painter.drawText(6, 12, p->displayName());
                painter.restore();
            }
        }
        pen.setWidthF(2.5);
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
        if(r->coordinates().containsPoint(f, Qt::WindingFill) && r->nr() != 0 && r->capacity() > 0) {
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
    if(newRoom != 0 && oldRoom != newRoom) {
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
//         QString message = "x=" + QString::number(mouseEvent->x()) + "; y=" + QString::number(mouseEvent->y());
//         statusBar()->showMessage(message);
        return;
    }
    movingPerson_->moveTo(mouseEvent->x(), mouseEvent->y());
    update();
}


bool MainWindow::event(QEvent* event)
{
    if(event->type() == QEvent::ToolTip) {
        QString tooltip;
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        foreach(Person*p, people_) {
            if(p->coordinates().containsPoint(helpEvent->pos(), Qt::WindingFill)) {
                tooltip = p->surname() + " " + p->name() + "\r\n";
                tooltip += "Department: " + p->department() + "\r\n";
                tooltip += "Team: " + p->team() + "\r\n";
                if(p->component().size() > 1){
                    tooltip += "Component: " + p->component() + "\r\n";
                }
                if(p->isLead()) {
                    tooltip += "Team Lead\r\n";
                }
                tooltip += "Room: " + QString::number(p->room());
            }
        }
        QToolTip::showText(helpEvent->globalPos(), tooltip);
    }
    return QWidget::event(event);
}
