#pragma once
#include <QComboBox>

class CComboBox : public QComboBox {
Q_OBJECT
public:
    CComboBox(QWidget *parent = 0) : QComboBox(parent) {}
    const QModelIndex index() {return index_;}
    void index(const QModelIndex &index) {index_ = index;}
private:
    QModelIndex index_;
};
