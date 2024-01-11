#include "LineEditRoomDelegate.h"
#include "RoomModel.h"
#include "constants.h"
#include "TableView.h"

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
            if(capacity > 0) {
                lineEdit->setEnabled(true);
            } else {
                lineEdit->setEnabled(false);
            }
            break;
        }
    }
}


QWidget *LineEditRoomDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option*/,
                                        const QModelIndex & /*index*/ ) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(true);

    return lineEdit;
}


void LineEditRoomDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/*index*/ ) const
{
    editor->setGeometry(option.rect);
}
