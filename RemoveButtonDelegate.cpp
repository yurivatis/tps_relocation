#include "RemoveButtonDelegate.h"
#include "ColorModel.h"
#include <QMouseEvent>

RemoveButtonDelegate::RemoveButtonDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}

bool RemoveButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *mouseEvent = static_cast< QMouseEvent* >( event );
        if( mouseEvent->buttons() & Qt::LeftButton )
        {
            ColorModel* colorModel = static_cast< ColorModel* >( model );
            colorModel->removeRow( index.row() );
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent( event, model, option, index );
}

void RemoveButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton button(index.data().toString());
    button.setGeometry(option.rect);
    painter->save();
    painter->translate(option.rect.topLeft());
    button.render(painter);
    painter->restore();
}
