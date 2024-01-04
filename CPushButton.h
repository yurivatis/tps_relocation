#pragma once
#include <QPushButton>
#include <QModelIndex>
#include <QColor>

class CPushButton : public QPushButton {
Q_OBJECT
public:
    CPushButton(QWidget *parent = 0) : QPushButton(parent) {}
    CPushButton(const QString &text, QWidget *parent = 0) : QPushButton(text, parent) {}
    const QModelIndex index() {return index_;}
    void index(const QModelIndex &index) {index_ = index;}
    QColor color() {return color_;}
    void color(QColor color) {color_ = color;}
private:
    QModelIndex index_;
    QColor color_ = Qt::gray;
};
