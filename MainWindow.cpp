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
#include <QPainterPath>
#include <QPixmap>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScrollArea>
#include <QToolTip>
#include <QLabel>
#include <QPageLayout>
#include "MainWindow.h"
#include "SQLite.h"
#include "LineEditDelegate.h"
#include "ComboboxDelegate.h"
#include "ColorButtonDelegate.h"
#include "RemoveButtonDelegate.h"
#include "constants.h"

MainWindow::MainWindow(QApplication *, QWidget *parent): QMainWindow(parent)
{
    paintWidget_ = new PaintWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(paintWidget_);
    paintWidget_->setFixedSize(1500, 1000);
//    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
//    setCentralWidget(mainWidget);
    QLabel *og1st = new QLabel(this);
    QFont lblF("Arial", 30, QFont::Bold);
    og1st->setFont(lblF);
    og1st->setText("1. OG");
    og1st->setGeometry(900, 50, 200, 200);
    
    helpWidget_ = new HelpWidget();
    helpWidget_->hide();
    
    colorModel_ = new ColorModel(SqlInterface::getInstance()->colorEntries() , (int)Column::TOTAL_COLUMNS);
    colorModel_->restore();

    colorFrame_ = new ColorFrame();
    colorFrame_->colorView_->setModel(colorModel_);
    
    ComboBoxDelegate *depd = new ComboBoxDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::DEPARTMENT, depd);

    ComboBoxDelegate *teamd = new ComboBoxDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::TEAM, teamd);

    ComboBoxDelegate *compd = new ComboBoxDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::COMPONENT, compd);

    ColorButtonDelegate *cold = new ColorButtonDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::COLOR, cold);

    RemoveButtonDelegate *remd = new RemoveButtonDelegate(colorFrame_->colorView_);
    colorFrame_->colorView_->setItemDelegateForColumn((int)Column::REMOVE, remd);

    toInitState();

    memberFrame_ = new MemberFrame;
    memberModel_ = new MemberModel(people_.size(), (int)MemberColumns::TOTAL_COLUMNS, this);
    proxyMemberModel_ = new SortFilterProxyModel(this);
    proxyMemberModel_->people(&people_);
    proxyMemberModel_->setSourceModel(memberModel_);
    memberFrame_->memberView_->setModel(proxyMemberModel_);

    LineEditDelegate *mdn = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::FULL_NAME, mdn);
    LineEditDelegate *mdl = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::LOCATION, mdl);
    LineEditDelegate *mdd = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::DEPARTMENT, mdd);
    LineEditDelegate *mdt = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::TEAM, mdt);
    LineEditDelegate *mdc = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::COMPONENT, mdc);
    LineEditDelegate *mdr = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::ROLE, mdr);
    LineEditDelegate *mdroom = new LineEditDelegate(memberFrame_->memberView_);
    memberFrame_->memberView_->setItemDelegateForColumn((int)MemberColumns::ROOM, mdroom);
    memberFrame_->memberView_->setSortingEnabled(true);
    memberFrame_->memberView_->sortByColumn((int)MemberColumns::FULL_NAME, Qt::AscendingOrder);
    createMenus();
    memberModel_->restore(&people_);

//    setFixedSize(1500, 1000);
    QObject::connect(depd, SIGNAL(oComboText(QString)), teamd, SLOT(getDepartment(QString)));
    QObject::connect(teamd, SIGNAL(oComboText(QString)), compd, SLOT(getTeam(QString)));
    QObject::connect(depd, SIGNAL(oComboChanged(QModelIndex,QString)), colorModel_, SLOT(setComboBox(QModelIndex,QString)));
    QObject::connect(teamd, SIGNAL(oComboChanged(QModelIndex,QString)), colorModel_, SLOT(setComboBox(QModelIndex,QString)));
    QObject::connect(compd, SIGNAL(oComboChanged(QModelIndex,QString)), colorModel_, SLOT(setComboBox(QModelIndex,QString)));
    QObject::connect(cold, SIGNAL(oColorChanged(QModelIndex&,QColor)), colorModel_, SLOT(setColor(QModelIndex&,QColor)));
    QObject::connect(colorFrame_, SIGNAL(oLoadDefault()), colorModel_, SLOT(loadDefault()));
    QObject::connect(colorFrame_, SIGNAL(oAddRow()), colorModel_, SLOT(addRow()));
    QObject::connect(colorFrame_, SIGNAL(oRestore()), colorModel_, SLOT(restore()));
    QObject::connect(colorFrame_, SIGNAL(oRemoveAll()), colorModel_, SLOT(removeAll()));
    QObject::connect(remd, SIGNAL(oRemoveRow(int)), colorModel_, SLOT(removeRow(int)));
    QObject::connect(colorFrame_, SIGNAL(oSave()), colorModel_, SLOT(save()));
    QObject::connect(colorModel_, SIGNAL(oUpdated()), this, SLOT(assignPeopleToRooms()));
    QObject::connect(memberFrame_, SIGNAL(oApply()), this, SLOT(updateMates()));

    QString message = tr("Detailed plan of Hacon's 1st floor");
    statusBar()->showMessage(message);
    setAttribute( Qt::WA_QuitOnClose, true);
    unstored_ = false;
    movingPerson_ = nullptr;
}


MainWindow::~MainWindow()
{
    delete helpWidget_;
    delete colorFrame_;
    delete memberFrame_;
    delete paintWidget_;
}


void MainWindow::createMenus()
{
    QMenu *dbMenu = menuBar()->addMenu(tr("&Database"));
    QMenu *membersMenu = menuBar()->addMenu(tr("&Members"));
    QMenu *customizeMenu = menuBar()->addMenu(tr("&Customize"));
    QMenu *printMenu  = menuBar()->addMenu(tr("&Screenshot"));
    QMenu *helpMenu  = menuBar()->addMenu(tr("&Help"));

    QAction *importDb = new QAction(tr("&Import from csv"), this);
    dbMenu->addAction(importDb);
    QObject::connect(importDb, SIGNAL(triggered()), this, SLOT(importDatabase()));

    QAction *exportCsv = new QAction(tr("&Export to csv"), this);
    dbMenu->addAction(exportCsv);
    QObject::connect(exportCsv, SIGNAL(triggered()), this, SLOT(exportCsv()));

    QAction *exportDb = new QAction(tr("&Export to db"), this);
    dbMenu->addAction(exportDb);
    QObject::connect(exportDb, SIGNAL(triggered()), this, SLOT(exportDatabase()));
    
    QAction *undoChanges = new QAction(tr("&Undo all changes"), this);
    dbMenu->addAction(undoChanges);
    QObject::connect(undoChanges, SIGNAL(triggered()), this, SLOT(toInitState()));
    
    QAction *edit = new QAction(tr("&Edit"), this);
    membersMenu->addAction(edit);
    QObject::connect(edit, SIGNAL(triggered()), this, SLOT(showMemberFrame()) );

    QAction * colors = new QAction(tr("&Colors"), this);
    customizeMenu->addAction(colors);
    QObject::connect(colors, SIGNAL(triggered()), this, SLOT(showColorFrame()));

    QAction *screenshot = new QAction(tr("&Take screenshot"), this);
    printMenu->addAction(screenshot);
    QObject::connect(screenshot, SIGNAL(triggered()), paintWidget_, SLOT(makeScreenshot()));
    
    QAction *about = new QAction(tr("&About"), this);
    helpMenu->addAction(about);
    QObject::connect(about, SIGNAL(triggered()), this, SLOT(showHelpWidget()));
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
    rooms_.append(new Room163(163, 3, {810, 900,  810, 800, 840, 800, 900, 860, 900, 900}));
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

    rooms_.append(new Room119(119, 4, { 490, 565,  587, 530,  610, 610,  610, 620, 505, 620}));
    rooms_.append(new Room(120, 4, { 475, 510,  572, 475,  587, 530,  490, 565}, Orientation::RIGHT, -18));

    rooms_.append(new Room(121, 0, { 355, 550,  444, 520,  462, 595,  345, 595}, Orientation::CENTER, 0));

    rooms_.append(new Room(149, 2, { 448, 435,  551, 400,  572, 475,  470, 511}, Orientation::RIGHT, -18));
    rooms_.append(new Room(148, 2, { 426, 359,  529, 324,  551, 400,  448, 435}, Orientation::RIGHT, -18));
    rooms_.append(new Room(147, 3, { 415, 321,  518, 286,  529, 324,  426, 359}, Orientation::RIGHT, -18));
    rooms_.append(new Room(146, 3, { 404, 283,  507, 248,  518, 286,  415, 321}, Orientation::RIGHT, -18));
    rooms_.append(new Room(145, 3, { 393, 245,  496, 210,  507, 248,  404, 283}, Orientation::RIGHT, -18));
    rooms_.append(new Room(144, 3, { 381, 205,  484, 170,  496, 210,  393, 245}, Orientation::RIGHT, -18));
    rooms_.append(new Room(143, 0, { 349,  93,  452,  58,  484, 170,  381, 205}, Orientation::RIGHT, -18));
    
    rooms_.append(new Room(  0, 0, { 181, 590,  181, 486}, Orientation::CENTER, -90, true));
    rooms_.append(new Room(131, 2, { 170, 448,  273, 413,  284, 451,  181, 486}, Orientation::LEFT, -18));
    rooms_.append(new Room(132, 2, { 159, 410,  262, 375,  273, 413,  170, 448}, Orientation::LEFT, -18));
    rooms_.append(new Room(133, 3, { 148, 372,  251, 337,  262, 375,  159, 410}, Orientation::LEFT, -18));
    rooms_.append(new Room(134, 3, { 137, 334,  240, 299,  251, 337,  148, 372}, Orientation::LEFT, -18));
    rooms_.append(new Room(135, 3, { 126, 296,  229, 261,  240, 299,  137, 334}, Orientation::LEFT, -18));
    rooms_.append(new Room(136, 3, { 115, 258,  218, 223,  229, 261,  126, 296}, Orientation::LEFT, -18));
    rooms_.append(new Room(137, 3, { 104, 220,  207, 185,  218, 223,  115, 258}, Orientation::LEFT, -19));
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
            if(r->nr() == curRoom && r->dummy() == false && r->capacity() > 0 && r->nr() != 0) {
                r->addPerson(p);
                p->room(curRoom);
                redrawMates(r);
                break;
            }
        }
    }
    update();
}


void MainWindow::redrawMates(Room* r)
{
    if(r->people_.size() == 0) {
        return;
    }
    r->redrawMates();
    paintWidget_->update();
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
    if(people_.size() == 0) {
        msgBox.setText(QString("Nothing to export"));
        msgBox.setIcon(QMessageBox::Warning);
    } else if(sqlInteface->exportToDb(people_) == true) {
        msgBox.setText(QString("Successfully exported"));
        msgBox.setIcon(QMessageBox::Information);
        unstored_ = false;
    } else {
        msgBox.setText(QString("Export failed"));
        msgBox.setIcon(QMessageBox::Warning);
    }
    msgBox.exec();
}


void MainWindow::exportCsv()
{
    QMessageBox msgBox;
    if(people_.size() == 0) {
        msgBox.setText(QString("Nothing to export"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(NULL, "Create New File","./", "*.csv");
    if(fileName.isEmpty()) {
        return;
    }
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    if(!f.isOpen()) {
        msgBox.setText(QString("Could not open file: %1").arg(fileName));
        msgBox.setIcon(QMessageBox::Critical);
        return;
    } else {
        QTextStream stream( &f );
        foreach(Person* p, people_) {
            stream << p->surname() << "," << p->name() << "," << p->location() << "," << p->department() << "," << p->team() << "," << p->role() << "," << p->component() << "," << p->room() << Qt::endl;
        }
        f.close();
    }
    msgBox.setText(QString("Successfully stored to: %1").arg(fileName));
    msgBox.setIcon(QMessageBox::Information);
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
            memberModel_->restore(&people_);
            unstored_ = false;
        } else {
            msgBox.setText(QString("Import from %1 failed").arg(fileName));
            msgBox.setIcon(QMessageBox::Warning);
        }
        msgBox.exec();
    }
}


void MainWindow::updateMates()
{
    foreach(Person *p, people_) {
        if(p->modified() != p->room()) {
            foreach(Room *r, rooms_) {
                if(r->nr() == p->room()) {
                    if(r->nr() != 0 && r->dummy() == false && r->capacity() > 0 && r->nr()) {
                        r->removePerson(p);
                    }
                    redrawMates(r);
                }
                if(r->nr() == p->modified()) {
                    if(r->nr() != 0 && r->dummy() == false && r->capacity() > 0) {
                        r->addPerson(p);
                        redrawMates(r);
                    }
                }
            }
            p->room(p->modified());
        }
    }
    unstored_ = true;
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    bool accepted = true;
    if(unstored_ == true) {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Close the application?");
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("You have unsaved changes. Close?"));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        const int ret = messageBox.exec();
        switch (ret) {
            case QMessageBox::Yes:
            break;
            case QMessageBox::No:
                accepted = false;
                event->ignore();
            break;
        }
    }
    if(accepted) {
        helpWidget_->close();
        colorFrame_->close();
        memberFrame_->close();
        event->accept();
    } else {
        event->ignore();
    }
}


void MainWindow::toInitState()
{
    people_.clear();
    rooms_.clear();
    addRooms();
    addPeople();
    assignPeopleToRooms();
    unstored_ = false;
}


void MainWindow::showHelpWidget()
{
    if(helpWidget_->isVisible()) {
        helpWidget_->close();
    }
    helpWidget_->show();
    helpWidget_->raise();
}


void MainWindow::showColorFrame()
{
    if(colorFrame_->isVisible()) {
        colorFrame_->close();
    }
    colorFrame_->show();
    colorFrame_->raise();
}


void MainWindow:: showMemberFrame()
{
    if(memberFrame_->isVisible()) {
        memberFrame_->close();
    }
    memberFrame_->show();
    memberFrame_->raise();
}

