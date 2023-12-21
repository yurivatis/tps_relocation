#include "ColorButtonDelegate.h"
#include "CPushButton.h"
#include "constants.h"
#include <QDebug>
#include <QMouseEvent>

ColorButtonDelegate::ColorButtonDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


QWidget *ColorButtonDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex & index ) const
{
    CPushButton *pb = new CPushButton(parent);
    pb->index(index);
    if(index.column() == (int)Column::COLOR) {
        QObject::connect(pb, SIGNAL(clicked()), this, SLOT(showColorDialog()));
    }
    pb->setAutoFillBackground(true);
    return pb;
}


void ColorButtonDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &) const
{
    CPushButton *pushButton = static_cast<CPushButton*>(editor);
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


void ColorButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    CPushButton *pb = static_cast<CPushButton*>(editor);
    QPalette palette = pb->palette();
    QColor color = palette.button().color();
    model->setData(index, color, Qt::BackgroundRole);
}


void ColorButtonDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void ColorButtonDelegate::showColorDialog()
{
    CPushButton *pb = qobject_cast<CPushButton*>(sender());
    if(pb != NULL) {
        QModelIndex index = pb->index();
        QColor color = QColorDialog::getColor(color);
        emit oColorChanged(index, color);
    }
}

