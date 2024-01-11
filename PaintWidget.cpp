#include <QPen>
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QToolTip>
#include <QFileDialog>
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
            painter.drawText(parent_->rooms()->at(i)->getCenter(), QString::number(parent_->rooms()->at(i)->nr()) + QString("/") + QString::number(parent_->rooms()->at(i)->capacity()));
        } else {
            painter.drawPolyline(parent_->rooms()->at(i)->coordinates());
        }
    }
}


bool PaintWidget::event(QEvent* event)
{
    bool accepted = true;
    if(event->type() == QEvent::ToolTip) {
        QString tooltip;
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        foreach(Person*p, *(parent_->people())) {
            if(p->coordinates().containsPoint(helpEvent->pos(), Qt::WindingFill)) {
                tooltip = p->surname() + " " + p->name() + "\r\n";
                tooltip += "Department: " + p->department() + "\r\n";
                tooltip += "Team: " + p->team() + "\r\n";
                if(p->component().size() > 1){
                    tooltip += "Component: " + p->component() + "\r\n";
                }
                tooltip += "Position: " + p->role() + "\r\n";
                tooltip += "Room: " + QString::number(p->room())/* + "\r\n"*/;
//                tooltip += "Location: " + p->location();
            }
        }
        QToolTip::showText(helpEvent->globalPos(), tooltip);
    }
    if(accepted) {
        return QWidget::event(event);
    } else {
        event->ignore();
        return false;
    }
}


void PaintWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton || parent_->moving() == nullptr) {
        return;
    }
    QPointF f(mouseEvent->position().x(), mouseEvent->position().y());
    foreach(Room *r, *(parent_->rooms())) {
        if(r->coordinates().containsPoint(f, Qt::WindingFill) && r->nr() != 0 && r->capacity() > 0) {
            parent_->moving()->modified(r->nr());
            parent_->updateMates();
            break;
        }
    }
    parent_->moving()->clear();
    parent_->moving(nullptr);
    update();
}


void PaintWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    if(parent_->moving() == nullptr) {
        return;
    }
    parent_->moving()->moveTo(mouseEvent->position().x(), mouseEvent->position().y());
    parent_->unstored(true);
    update();
}


void PaintWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    parent_->moving(nullptr);
    QPointF f(mouseEvent->position().x(), mouseEvent->position().y());
    if(mouseEvent->button() != Qt::LeftButton) {
        return;
    }
    foreach(Person*p, *(parent_->people())) {
        if(p->coordinates().containsPoint(f, Qt::WindingFill)) {
            parent_->moving(p);
            p->offset(f.x(), f.y());
            break;
        }
    }
}



void PaintWidget::makeScreenshot()
{
    auto const pm = this->grab();

    QString initialPath = "hacon_first_floor";
    QString sFormat = "png";
    QString sFullPath = initialPath;
    sFullPath.append(".");
    sFullPath.append(sFormat);

    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                     sFullPath,
                                                     tr("%1 Files (*.%2);;All Files (*)")
                                                         .arg(sFormat.toUpper())
                                                         .arg(sFormat));
    pm.save(sFileName);
}
