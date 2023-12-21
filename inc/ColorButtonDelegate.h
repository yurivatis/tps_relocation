#pragma once
#include <QString>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QColorDialog>
#include <QColor>
#include <QEvent>

class PushButtonDelegate : public QStyledItemDelegate
{
     Q_OBJECT

public:
    PushButtonDelegate(const QString &text, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
private slots:
    void showColorDialog();
private:
    QString text_;
signals:
    void oColor(QColor);
};
