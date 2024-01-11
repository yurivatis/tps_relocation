#pragma once
#include <QString>
#include <QLineEdit>
#include <QStyledItemDelegate>


class LineEditRoomDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    LineEditRoomDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};
