#pragma once
#include <QString>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QColorDialog>
#include <QColor>
#include <QEvent>
#include <QPainter>

class RemoveButtonDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    RemoveButtonDelegate(QObject *parent = nullptr);

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
