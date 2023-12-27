#include "MemberFrame.h"
#include "TableView.h"
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>

MemberFrame::MemberFrame(QWidget *parent) : QTabWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    memberView_ = new TableView(this);
    vl->addWidget(memberView_);
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
}


