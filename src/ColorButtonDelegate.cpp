#include "PushButtonDelegate.h"
#include "constants.h"
#include <QDebug>
#include <QMouseEvent>

PushButtonDelegate::PushButtonDelegate(const QString &text, QObject *parent)
     : QStyledItemDelegate(parent)
{
    text_ = text;
}


QWidget *PushButtonDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex & index ) const
{
    QPushButton *editor = new QPushButton(parent);
    editor->setAutoFillBackground(true);
    return editor;
}


void PushButtonDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QPushButton *pushButton = static_cast<QPushButton*>(editor);
    switch(index.column()) {
        case (int)Column::COLOR:
            QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showColorDialog()));
        case (int)Column::ADD_NEW:
        case (int)Column::REMOVE:
        {
            pushButton->setText(text_);
            break;
        }
    }
}


void PushButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QPushButton *pushButton = static_cast<QPushButton*>(editor);
    QString value = pushButton->text();
    model->setData(index, value, Qt::DisplayRole);
}


void PushButtonDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void PushButtonDelegate::showColorDialog()
{
    QPushButton *pb = static_cast<QPushButton*>(sender());
    pb->repaint();
    QPalette palette = pb->palette();
    QColor color = palette.button().color();
    color = QColorDialog::getColor(color);
    palette.setColor(QPalette::Button, color);
//    pb->repaint();
    emit oColor(color);
}

