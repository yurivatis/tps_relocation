#pragma once
#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>

class HelpWidget : public QWidget {
Q_OBJECT
public:
	HelpWidget(QWidget * parent = 0);
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent *);
private:
    QRect pos_;
};
