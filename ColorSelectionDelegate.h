#pragma once

#include <QString>
#include <QStyledItemDelegate>
#include <QColorDialog>
#include <QColor>

class ColorSelectionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ColorSelectionDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};
