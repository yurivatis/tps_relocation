#include "ColorFrame.h"
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>

ColorFrame::ColorFrame(QWidget *parent) : QTabWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    colorView_ = new ColorView(this);
    vl->addWidget(colorView_);
    QFrame *f = new QFrame(this);
    vl->addWidget(f);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
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
}


