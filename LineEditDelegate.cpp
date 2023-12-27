#include "LineEditDelegate.h"
#include "CComboBox.h"
#include "constants.h"
#include "MainWindow.h"
#include "SQLite.h"
#include <QDebug>
#include "TableView.h"
#include "ColorModel.h"

LineEditDelegate::LineEditDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


void LineEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    TableView *mv = (TableView *)parent();
    MemberModel *mm = (MemberModel *)mv->model();
    switch(index.column()) {
        case (int)MemberColumn::NAME:
        {
            lineEdit->setText(mm->people()->at(index.row())->displayFullName());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::ROOM:
        {
            lineEdit->setText(QString::number(mm->people()->at(index.row())->room()));
            lineEdit->setEnabled(true);
            break;
        }
    }
}


QWidget *LineEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option*/,
                                        const QModelIndex & /*index*/ ) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(true);

    return lineEdit;
}


void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    TableView *mv = (TableView *)parent();
    MemberModel *mm = (MemberModel *)mv->model();
    QString value = lineEdit->text();
    mm->people()->at(index.row())->modified(value.toInt());
    model->setData(index, value, Qt::DisplayRole);
}


void LineEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/*index*/ ) const
{
    editor->setGeometry(option.rect);
}

