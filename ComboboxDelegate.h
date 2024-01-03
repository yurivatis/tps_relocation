#pragma once
#include <QString>
#include <QComboBox>
#include <QStyledItemDelegate>


class ComboBoxDelegate : public QStyledItemDelegate
{
     Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
signals:
    void oComboText(const QString) const;
    void oComboChanged(QModelIndex index, QString) const;
public slots:
    void getDepartment(const QString);
    void getTeam(const QString);
private:
    QString department_;
    QString team_;
};
