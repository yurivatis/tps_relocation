#include "MainWindow.h"
#include "constants.h"
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QPolygon>
#include <QPoint>
#include <QPen>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
#include "SQLite.h"

MainWindow::MainWindow(QApplication *app, QWidget *parent): QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    createMenus();
    addRooms();
    drawFirstFloor();
    colorModel_ = new ColorModel(1, 6);

    colorView_ = new ColorView();
    colorView_->setWindowModality(Qt::ApplicationModal);
    colorView_->setModel(colorModel_);
    colorView_->setWindowTitle(QObject::tr("Customize colors"));

    departmentDelegate_ = new ComboBoxDelegate();
    colorView_->setItemDelegateForColumn((int)Column::DEPARTMENT, departmentDelegate_);

    teamDelegate_ = new ComboBoxDelegate(colorView_);
    colorView_->setItemDelegateForColumn((int)Column::TEAM, teamDelegate_);

    componentDelegate_ = new ComboBoxDelegate(colorView_);
    colorView_->setItemDelegateForColumn((int)Column::COMPONENT, componentDelegate_);

    colorDelegate_ = new PushButtonDelegate("", colorView_);
    colorView_->setItemDelegateForColumn((int)Column::COLOR, colorDelegate_);

//    setMinimumSize(1500, 1000);
    QObject::connect(departmentDelegate_, SIGNAL(oComboText(const QString)), teamDelegate_, SLOT(getDepartment(const QString)));
    QObject::connect(teamDelegate_, SIGNAL(oComboText(const QString)), componentDelegate_, SLOT(getTeam(const QString)));
    QObject::connect(departmentDelegate_, SIGNAL(oComboChanged(const QModelIndex &, const QString)), colorModel_, SLOT(setComboBox(const QModelIndex&, const QString)));

    QString message = tr("Detailed plan of Hacon's 1st floor");
    statusBar()->showMessage(message);
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
    rooms_.append(new Room(  0, 0, { 900, 900,  900, 960, 980, 960}, true));
    rooms_.append(new Room(176, 4, { 980, 960,  980, 860, 1060, 860, 1060, 960}));
    rooms_.append(new Room(175, 2, {1060, 960, 1060, 900, 1120, 900, 1120, 960}));
    rooms_.append(new Room(173, 2, {1120, 960, 1120, 900, 1180, 900, 1180, 960}));
    rooms_.append(new Room(174, 0, {1070, 900, 1070, 860, 1170, 860, 1170, 900}));
    rooms_.append(new Room(172, 4, {1180, 960, 1180, 860, 1260, 860, 1260, 960}));
    rooms_.append(new Room(171, 2, {1260, 960, 1260, 860, 1330, 860, 1330, 960}));
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
    colorView_->show();
}


void MainWindow::drawFirstFloor()
{

}


void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for(int i = 0; i < rooms_.size(); ++i) {
        if(!rooms_.at(i)->dummy()) {
            painter.drawPolygon(rooms_.at(i)->coordinates());
            painter.drawText(rooms_.at(i)->getCenter(), QString::number(rooms_.at(i)->nr()));
        } else {
            painter.drawPolyline(rooms_.at(i)->coordinates());
        }
    }
}


