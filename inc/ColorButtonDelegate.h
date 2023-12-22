#pragma once
#include <QString>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QColorDialog>
#include <QColor>
#include <QEvent>

class ColorButtonDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    ColorButtonDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

void setEditorData(QWidget *editor, const QModelIndex &) const;
void setModelData(QWidget *editor, QAbstractItemModel *model,
const QModelIndex &index) const override;

void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
private slots:
    void showColorDialog();
signals:
    void oColorChanged(const QModelIndex, const QColor) const;
};
