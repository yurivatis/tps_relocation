#include "MemberFrame.h"
#include "TableView.h"
#include <QHeaderView>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

MemberFrame::MemberFrame(QWidget *parent) : QTabWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    QFrame *searchFrame = new QFrame(this);
    QHBoxLayout *hlSearch = new QHBoxLayout;
    searchFrame->setLayout(hlSearch);
    QLabel *l = new QLabel(QString(tr("Search:")), this);
    QLineEdit *searchField = new QLineEdit(searchFrame);
    hlSearch->addWidget(l);
    hlSearch->addWidget(searchField);
    vl->addWidget(searchFrame);
    memberView_ = new TableView(this);
    vl->addWidget(memberView_);
    QFrame *f = new QFrame(this);
    vl->addWidget(f);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    QPushButton *apply = new QPushButton(tr("Apply"), f);
    hl->addWidget(apply);
    QObject::connect(apply, SIGNAL(clicked()), this, SIGNAL(oApply()));
    QPushButton *close = new QPushButton(tr("Close"), f);
    hl->addWidget(close);
    QObject::connect(close, SIGNAL(clicked()), this, SLOT(close()));
    f->setLayout(hl);
    
    setLayout(vl);
    setWindowTitle(QObject::tr("Hacon members"));
    pos_ = QRect(50, 150, 700, 300);
    QObject::connect(searchField, SIGNAL(textChanged(const QString &)), this, SIGNAL(oSearchChanged(const QString&)));
}


void MemberFrame::closeEvent(QCloseEvent *event)
{
    pos_ = this->geometry();
    event->accept();
}


void MemberFrame::showEvent(QShowEvent *event)
{
    this->setGeometry(pos_);
    event->accept();
}

