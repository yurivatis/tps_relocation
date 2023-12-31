#include "ColorFrame.h"
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>

ColorFrame::ColorFrame(QWidget *parent) : QTabWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    colorView_ = new TableView(this);
    vl->addWidget(colorView_);
    QFrame *f = new QFrame(this);
    vl->addWidget(f);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    QPushButton *loadDefault = new QPushButton(tr("Load default"), f);
    hl->addWidget(loadDefault);
    QObject::connect(loadDefault, SIGNAL(clicked()), this, SIGNAL(oLoadDefault()));
    QPushButton *remove = new QPushButton(tr("Remove all"), f);
    hl->addWidget(remove);
    QObject::connect(remove, SIGNAL(clicked()), this, SIGNAL(oRemoveAll()));
    QPushButton *restore = new QPushButton(tr("Restore"), f);
    hl->addWidget(restore);
    QObject::connect(restore, SIGNAL(clicked()), this, SIGNAL(oRestore()));
    QPushButton *add = new QPushButton(tr("Add row"), f);
    hl->addWidget(add);
    QObject::connect(add, SIGNAL(clicked()), this, SIGNAL(oAddRow()));
    QPushButton *save = new QPushButton(tr("Save"), f);
    hl->addWidget(save);
    QObject::connect(save, SIGNAL(clicked()), this, SIGNAL(oSave()));
    QPushButton *close = new QPushButton(tr("Close"), f);
    hl->addWidget(close);
    QObject::connect(close, SIGNAL(clicked()), this, SLOT(close()));
    f->setLayout(hl);
    setLayout(vl);
    setWindowTitle(QObject::tr("Customize colors"));
    pos_ = QRect(150, 150, 600, 300);
}


void ColorFrame::closeEvent(QCloseEvent *event)
{
    pos_ = this->geometry();
    event->accept();
}


void ColorFrame::showEvent(QShowEvent *event)
{
    this->setGeometry(pos_);
    event->accept();
}
