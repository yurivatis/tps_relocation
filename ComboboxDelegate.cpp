#include "ComboboxDelegate.h"
#include "CComboBox.h"
#include "constants.h"
#include "SQLite.h"
#include <QDebug>
#include "TableView.h"
#include "ColorModel.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


 QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex & index ) const
{
    CComboBox *comboBox = new CComboBox(parent);
    comboBox->index(index);
    comboBox->setFrame(true);

    return comboBox;
}


void ComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    CComboBox *comboBox = static_cast<CComboBox*>(editor);
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    TableView *cv = (TableView *)parent();
    ColorModel *cm = (ColorModel *)cv->model();
    switch(index.column()) {
        case (int)Column::DEPARTMENT:
        {
            QStringList list = sqlInteface->departments();
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
            break;
        }
        case (int)Column::TEAM:
        {
            QModelIndex idx = cm->index(index.row(), (int)Column::DEPARTMENT);
            QVariant dep = cm->data(idx, Qt::DisplayRole);
            QStringList list = sqlInteface->teams(dep.toString());
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
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
}


void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &) const
{
    CComboBox *comboBox = static_cast<CComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(comboBox->index(), value, Qt::DisplayRole);
    emit oComboText(value);
    emit oComboChanged(comboBox->index(), value);
}


void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


void ComboBoxDelegate::getDepartment(const QString department)
{
    department_ = department;
}


void ComboBoxDelegate::getTeam(const QString team)
{
    team_ = team;
}
