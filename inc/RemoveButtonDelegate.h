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

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
    void oCheckRemove();
signals:
    void oRemoveRow(int);
};
