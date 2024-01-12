#include "LineEditDelegate.h"
#include "MemberModel.h"
#include "constants.h"
#include <QSortFilterProxyModel>
#include "TableView.h"

LineEditDelegate::LineEditDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
{
}


void LineEditDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    TableView *mv = (TableView *)parent();
    SortFilterProxyModel *mm = (SortFilterProxyModel *)mv->model();
    if(mm->columnCount() <= 0 || mm->people()->size() <= 0) {
        return;
    }
    QString fullName = mm->data(mm->index(index.row(), (int)MemberColumns::FULL_NAME)).toString();
    Person *person = nullptr;
    foreach(Person *p, *(mm->people())) {
        if(p->displayFullName() == fullName) {
            person = p;
            break;
        }
    }
    if(person == nullptr) {
        return;
    }

    switch(index.column()) {
        case (int)MemberColumn::FULL_NAME:
        {
            lineEdit->setText(person->displayFullName());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::DEPARTMENT:
        {
            lineEdit->setText(person->department());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::TEAM:
        {
            lineEdit->setText(person->team());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::ROLE:
        {
            lineEdit->setText(person->role());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::COMPONENT:
        {
            lineEdit->setText(person->component());
            lineEdit->setEnabled(false);
            break;
        }
        case (int)MemberColumn::ROOM:
        {
            lineEdit->setText(QString::number(person->room()));
            lineEdit->setEnabled(true);
            break;
        }
    }
}


QWidget *LineEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option*/,
                                        const QModelIndex & index ) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    if(index.column() == (int) MemberColumn::ROOM) {
        lineEdit->setValidator(new QIntValidator(0, 600));
    }
    lineEdit->setFrame(true);

    return lineEdit;
}


void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    SortFilterProxyModel *mm = (SortFilterProxyModel*)model;
    QString value = lineEdit->text();
    if(!mm || !mm->people() || mm->people()->size() <= 0) {
        return;
    }
    QString fullName = model->data(model->index(index.row(), (int)MemberColumns::FULL_NAME)).toString();
    foreach(Person *p, *(mm->people())) {
        if(p->displayFullName() == fullName) {
            p->modified(value.toInt());
            break;
        }
    }
    model->setData(index, value, Qt::DisplayRole);
}


void LineEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &/*index*/ ) const
{
    editor->setGeometry(option.rect);
}

