#include "ComboboxDelegate.h"
#include "constants.h"
#include "SQLite.h"
#include "TableView.h"
#include "ColorModel.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


 QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index*/ ) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setFrame(true);
    return comboBox;
}


void ComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index ) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    TableView *cv = (TableView *)parent();
    ColorModel2 *cm = (ColorModel2 *)cv->model();

    switch(index.column()) {
        case (int)Column::DEPARTMENT:
        {
            QStringList list = sqlInteface->departments();
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }

            // set index to current selection
            const auto data = cm->data( index, Qt::DisplayRole ).toString();
            int index = comboBox->findText( data );
            if( index != -1 )
                comboBox->setCurrentIndex( index );
            break;
        }
        case (int)Column::TEAM:
        {
            QModelIndex idx = cm->index(index.row(), (int)Column::DEPARTMENT);
            QVariant dep = cm->data(idx, Qt::DisplayRole);
            QStringList list = sqlInteface->teams( dep.toString() );
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
            // set index to current selection
            const auto data = cm->data( index, Qt::DisplayRole ).toString();
            int index = comboBox->findText( data );
            if( index != -1 )
                comboBox->setCurrentIndex( index );
            break;
        }
        case (int)Column::COMPONENT:
        {
            QModelIndex idx = cm->index(index.row(), (int)Column::DEPARTMENT);
            QVariant dep = cm->data(idx, Qt::DisplayRole);
            idx = cm->index(index.row(), (int)Column::TEAM);
            QVariant team = cm->data(idx, Qt::DisplayRole);
            QStringList list = sqlInteface->components(dep.toString(), team.toString());
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
            break;
        }
    }
    comboBox->showPopup();
}


void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index ) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData( index, value, Qt::DisplayRole );
}


void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
