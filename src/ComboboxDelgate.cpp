#include "ComboboxDelgate.h"
#include "constants.h"
#include "SQLite.h"
#include <QDebug>


ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


 QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index */) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setFrame(true);

    return comboBox;
}


void ComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    SqlInterface *sqlInteface = SqlInterface::getInstance();
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
            QStringList list = sqlInteface->teams(department_);
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
            break;
        }
        case (int)Column::COMPONENT:
        {
            QStringList list = sqlInteface->components(team_);
            for(int i = 0; i < list.size(); i++) {
                comboBox->addItem(list.at(i));
            }
            break;
        }
    }
}


void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::DisplayRole);
    emit oComboText(comboBox->currentText());
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


