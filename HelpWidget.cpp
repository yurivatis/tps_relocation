
#include "HelpWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>

HelpWidget::HelpWidget(QWidget *parent): QWidget(parent)
{
	this->setWindowTitle(tr("About tps_relocation"));
	QVBoxLayout *l = new QVBoxLayout(this);
	QFrame *name = new QFrame(this);
	l->addWidget(name);
	QHBoxLayout *hl=new QHBoxLayout(name);
    QString version = QString("<h2> tps_relocation 1.4 </h2>");
	version.append(" </h2>");
	QLabel *nameLabel = new QLabel(version, name);
	hl->addWidget(nameLabel, Qt::AlignLeft|Qt::AlignBottom);
	QTabWidget *tw = new QTabWidget(this);
	l->addWidget(tw);
	QWidget *w=new QWidget(tw);
	tw->addTab(w, tr("About"));
	QVBoxLayout *vl= new QVBoxLayout(w);
	QLabel *lbl = new QLabel(w);
 	lbl->setText(tr("tps_relocation is a small application allowing visualization and movement <br>"
					" of all Hacon TPS members located on the 1st floor.<br><br>"
					"<b>Current limitations:</b> To add a new member or to remove an existing member <br>"
					"you have to modify attached csv file and to import it again.<br>"));
	lbl->setTextFormat(Qt::RichText);
	lbl->setOpenExternalLinks(true);
	vl->addWidget(lbl);

	w = new QWidget(tw);
	tw->addTab(w, tr("Authors"));
	vl= new QVBoxLayout(w);
	lbl = new QLabel(w);
	lbl->setText(tr("Yuri Vatis<br><br><a href = mailto:yuri.vatis@hacon.de>yuri.vatis@hacon.de</a>"));
	lbl->setTextFormat(Qt::RichText);
	lbl->setOpenExternalLinks(true);
	vl->addWidget(lbl);

	QPushButton *closeButton = new QPushButton(tr("Close"), this);
	l->addWidget(closeButton, 0, Qt::AlignRight);
	connect(closeButton, SIGNAL(released()), this, SLOT(close()));
}
