#include <QPen>
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include "Person.h"
#include "PaintWidget.h"
#include "Room.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    parent_ = (MainWindow*)parent;
}


void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    QColor color;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(parent_ == nullptr) {
        return;
    }
    for(int i = 0; i < parent_->rooms()->size(); ++i) {
        int room_nr = parent_->rooms()->at(i)->nr();
        for(int j = 0; j < parent_->rooms()->at(i)->people_.size(); j++) {
            Person *p = parent_->rooms()->at(i)->people_.at(j);
            color = p->color();
            pen.setWidthF(0.5);
            pen.setColor(Qt::white);
            painter.setPen(pen);
            QPainterPath path;
            if(p == parent_->moving()) {
                path.addPolygon(p->tmpcoordinates());
                color.setAlpha(127);
                p->tmpcoordinates();
            } else {
                path.addPolygon(p->coordinates());
                color.setAlpha(255);
            }
            brush.setColor(color);
            brush.setStyle(Qt::SolidPattern);
            painter.fillPath(path, brush);
            painter.drawPolygon(p->coordinates());
            if(room_nr != 0 && !parent_->rooms()->at(i)->dummy() && p) {
                painter.save();
                pen.setColor(Qt::black);
                painter.setPen(pen);
                painter.translate(p->coordinates().at(0).x(), p->coordinates().at(0).y());
                painter.rotate(parent_->rooms()->at(i)->rotation());
                if(p->isLead()) {
                    painter.setFont(QFont("Helvetica", 9, QFont::ExtraBold));
                }
                painter.drawText(6, 12, p->displayName());
                painter.restore();
            }
        }
        pen.setWidthF(2.5);
        pen.setColor(Qt::black);
        painter.setPen(pen);
        if(!parent_->rooms()->at(i)->dummy()) {
            painter.drawPolygon(parent_->rooms()->at(i)->coordinates());
            painter.drawText(parent_->rooms()->at(i)->getCenter(), QString::number(parent_->rooms()->at(i)->nr()));
        } else {
            painter.drawPolyline(parent_->rooms()->at(i)->coordinates());
        }
    }
}
