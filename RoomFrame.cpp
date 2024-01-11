#include "RoomFrame.h"
#include "TableView.h"
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>

RoomFrame::RoomFrame(QWidget *parent) : QTabWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    roomView_ = new TableView(this);
    vl->addWidget(roomView_);
    QFrame *f = new QFrame(this);
    vl->addWidget(f);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    QPushButton *save = new QPushButton(tr("Save"), f);
    hl->addWidget(save);
    QObject::connect(save, SIGNAL(clicked()), this, SIGNAL(oSave()));
    QPushButton *close = new QPushButton(tr("Close"), f);
    hl->addWidget(close);
    QObject::connect(close, SIGNAL(clicked()), this, SLOT(close()));
    f->setLayout(hl);
    
    setLayout(vl);
    setWindowTitle(QObject::tr("Hacon Rooms"));
    pos_ = QRect(50, 150, 700, 300);
}


void RoomFrame::closeEvent(QCloseEvent *event)
{
    pos_ = this->geometry();
    event->accept();
}


void RoomFrame::showEvent(QShowEvent *event)
{
    this->setGeometry(pos_);
    event->accept();
}

