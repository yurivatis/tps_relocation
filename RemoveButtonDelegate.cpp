#include "RemoveButtonDelegate.h"
#include "CPushButton.h"
#include "ColorModel.h"
#include "constants.h"
#include <QMouseEvent>

RemoveButtonDelegate::RemoveButtonDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


QWidget *RemoveButtonDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex & index ) const
{
    CPushButton *pb = new CPushButton(tr("Remove row"), parent);
    pb->index(index);
    QObject::connect(pb, SIGNAL(clicked()), this, SLOT(oCheckRemove()));
    pb->setCheckable(true);
    pb->setChecked(true);
    pb->setEnabled(true);
    return pb;
}


void RemoveButtonDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    if(index.column() == (int)Column::REMOVE) {
        const auto pb = dynamic_cast<CPushButton*>(editor);
        pb->setChecked(true);
        pb->setText(value);
        pb->setCheckable(true);
    }
//     if (index.model()->headerData(index.column(), Qt::Horizontal, Qt::DisplayRole).toString() == constants::kUse)
//     {
//         const auto value = index.model()->data(index, Qt::EditRole).toString();
//         const auto pb = dynamic_cast<QPushButton*>(editor);
//         pb->setChecked(value == "Yes" ? true : false);
//         pb->setText(value);
//         pb->setCheckable(true);
//     }
//     switch(index.column()) {
//         case (int)Column::COLOR:
//             QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showColorDialog()));
// 
//             break;
//         case (int)Column::ADD_NEW:
//         case (int)Column::REMOVE:
//         {
//             pushButton->setText("Remove Row");
//             break;
//         }
//     }
}


void RemoveButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    CPushButton *pb = static_cast<CPushButton*>(editor);
    QPalette palette = pb->palette();
    QColor color = palette.button().color();
    model->setData(index, color, Qt::EditRole);
    pb->setChecked(true);
    pb->setCheckable(true);
    const auto ptr = dynamic_cast<ColorModel*>(model);
    ptr->setData(index, "", Qt::EditRole);
}


void RemoveButtonDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void RemoveButtonDelegate::oCheckRemove()
{
    CPushButton *pb = qobject_cast<CPushButton*>(sender());
    if(pb != NULL) {
        QModelIndex index = pb->index();
        emit oRemoveRow(index.row());
    }
}


void RemoveButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton button(index.data().toString());
    button.setGeometry(option.rect);
    button.setText(button.text());
    painter->save();
    painter->translate(option.rect.topLeft());
    button.render(painter);
    painter->restore();
}
