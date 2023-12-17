#pragma once
#include <QPushButton>
#include <QModelIndex>

class CPushButton : public QPushButton {
Q_OBJECT
public:
    CPushButton(QWidget *parent = 0) : QPushButton(parent) {}
    const QModelIndex index() {return index_;}
    void index(const QModelIndex &index) {index_ = index;}
private:
    QModelIndex index_;
};
