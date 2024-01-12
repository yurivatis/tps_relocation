#include "LineEditRoomDelegate.h"
#include "RoomModel.h"
#include "constants.h"
#include "TableView.h"
#include <QIntValidator>

LineEditRoomDelegate::LineEditRoomDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


void LineEditRoomDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    TableView *mv = (TableView *)parent();
    SortRoomProxyModel *mm = (SortRoomProxyModel *)mv->model();
    if(mm->columnCount() <= 0 || mm->rooms()->size() <= 0) {
        return;
    }
    int roomNr = mm->data(mm->index(index.row(), (int)RoomColumns::NUMBER)).toInt();
    Room *room = nullptr;
    foreach(Room *r, *(mm->rooms())) {
        if(r->nr() == roomNr) {
            room = r;
            break;
        }
    }
    if(room == nullptr) {
        return;
    }
    QString val;
    switch(index.column()) {
        case (int)RoomColumn::NUMBER:
        {
            lineEdit->setText(val.setNum(roomNr));
            lineEdit->setEnabled(false);
            break;
        }
        case (int)RoomColumn::CAPACITY:
        {
            int capacity = room->capacity();
            lineEdit->setText(val.setNum(capacity));
            if(room->serviceRoom() == false) {
                lineEdit->setEnabled(true);
            } else {
                lineEdit->setEnabled(false);
            }
            break;
        }
    }
}


void LineEditRoomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    SortRoomProxyModel *mm = (SortRoomProxyModel*)model;
    QString value = lineEdit->text();
    if(!mm || !mm->rooms() || mm->rooms()->size() <= 0) {
        return;
    }
    int roomNr = model->data(model->index(index.row(), (int)RoomColumns::NUMBER)).toInt();
    foreach(Room *r, *(mm->rooms())) {
        if(r->nr() == roomNr) {
            r->capacity(value.toInt());
            break;
        }
    }
    model->setData(index, value, Qt::DisplayRole);
}


QWidget *LineEditRoomDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option*/,
                                        const QModelIndex & index ) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    if(index.column() == (int)RoomColumn::CAPACITY) {
        lineEdit->setValidator(new QIntValidator(0, 10));
    }
    lineEdit->setFrame(true);

    return lineEdit;
}


void LineEditRoomDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/*index*/ ) const
{
    editor->setGeometry(option.rect);
}
