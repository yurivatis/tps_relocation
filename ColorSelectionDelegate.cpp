#include "ColorSelectionDelegate.h"

ColorSelectionDelegate::ColorSelectionDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


QWidget *ColorSelectionDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex & /*index*/ ) const
{
    QColorDialog *dlg = new QColorDialog( parent );
    return dlg;
}


void ColorSelectionDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index ) const
{
    QColorDialog *dlg = static_cast<QColorDialog*>( editor );
    auto mdl = index.model();
    auto clr = mdl->data( index, Qt::BackgroundRole ).value< QColor >();
    dlg->setCurrentColor( clr );
}


void ColorSelectionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QColorDialog *dlg = static_cast<QColorDialog*>( editor );
    model->setData( index, dlg->selectedColor(), Qt::BackgroundRole );
}


void ColorSelectionDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
