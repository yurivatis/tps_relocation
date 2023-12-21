#include "ColorModel.h"
#include "CComboBox.h"
#include "CPushButton.h"
#include "constants.h"
#include "SQLite.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QColor>

ColorModel::ColorModel(int rows, int cols, QObject *parent)
    : QStandardItemModel(rows, cols, parent)
{
    columns_ = cols;
    rows_ = rows;
}


int ColorModel::rowCount(const QModelIndex & /*parent*/) const
{
   return rows_;
}


int ColorModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columns_;
}


QVariant ColorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == (int)Column::DEPARTMENT) {
            return QString("Department");
        } else if (section == (int)Column::TEAM) {
            return QString("Team");
        } else if (section == (int)Column::COMPONENT) {
            return QString("Component");
        } else if (section == (int)Column::COLOR) {
            return QString("Color");
        } else if (section == (int)Column::REMOVE) {
            return QString("Remove row");
        }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return QString("%1").arg(section + 1);
    }
    return QVariant();
}


QVariant ColorModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch(role) {
        case Qt::BackgroundRole:
        {
            if(index.column() == (int)Column::COLOR) {
                foreach(ModelValue item, list_) {
                    if(index.column() == item.col && index.row() == item.row) {
                        value = item.color;
                        break;
                    }
                }
            }
            break;
        }
        case Qt::ForegroundRole:
        {
            if(index.column() == (int)Column::REMOVE) {
                value = QString(tr("Remove row"));
            }
            break;
        }
        case Qt::DisplayRole:
        {
            foreach(ModelValue item, list_) {
                if(index.column() == item.col && index.row() == item.row) {
                    value = item.value;
                    break;
                }
            }
            break;
        }
    }
    return value;
}


void ColorModel::setComboBox(const QModelIndex& index, const QString value)
{
    ModelValue mv = {index.row(), index.column(), value, Qt::gray};

    for( ModelListIterator it( list_ ); it.hasNext(); ) {
        auto myObj( it.next() );
        if( myObj.row == mv.row && myObj.col >= mv.col && myObj.col < (int)Column::COLOR) {
            it.remove();  
        }
    }
    list_.append(mv);
}


void ColorModel::setColor(const QModelIndex index, const QColor color)
{
    ModelValue mv = {index.row(), index.column(), "", color};
    for( ModelListIterator it( list_ ); it.hasNext(); ) {
        auto myObj( it.next() );
        if( myObj.row == mv.row && myObj.col == (int)Column::COLOR) {
            it.remove();  
        }
    }
    list_.append(mv);
}

void ColorModel::addRow()
{
    setRowCount(++rows_);
}


void ColorModel::removeRow(int row)
{
    removeRows(row, 1);
    --rows_;
    for( ModelListIterator it( list_ ); it.hasNext(); ) {
        auto myObj( it.next() );
        if( myObj.row == row ) {
            it.remove();  
        } else if(myObj.row > row) {
            it.value().row--;
        }
    }
}


void ColorModel::save()
{
    bool ret = true;
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    sqlInteface->clearColorTable();

    for(int i = 0; i < rows_; i++) {
        QString department, team, component;
        QColor color;
        for(ModelListIterator it( list_ ); it.hasNext(); ) {
            auto myObj( it.next() );
            if( myObj.row == i ) {
                switch(myObj.col) {
                    case (int)Column::DEPARTMENT:
                        department = myObj.value;
                        break;
                    case (int)Column::TEAM:
                        team = myObj.value;
                        break;
                    case (int)Column::COMPONENT:
                        component = myObj.value;
                        break;
                    case (int)Column::COLOR:
                        color = myObj.color;
                        break;
                }
            }
        }
        if(sqlInteface->writeColor(department, team, component, color) == false) {
            ret = false;
            break;
        }
    }
    QMessageBox msgBox;
    if(ret == true) {
        msgBox.setText(QString("Successfully saved colors"));
        msgBox.setIcon(QMessageBox::Information);
    } else {
        msgBox.setText(QString("Saving colors to db failed"));
        msgBox.setIcon(QMessageBox::Warning);
    }
    emit oUpdated();
    msgBox.exec();
}


int ColorModel::restore()
{
    ModelValue mv;
    const int query_size = 7;
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QStringList sl = sqlInteface->getColorTable();
    int rows = sl.size() / query_size;
    int i = 0;

    while(i < sl.size()) {
        int cur_row = i / query_size;
        mv.row = cur_row;
        mv.col = 0;
        mv.value = sl.at(i);
        list_.append(mv);
        mv.col = 1;
        mv.value = sl.at(i+1);
        list_.append(mv);
        mv.col = 2;
        mv.value = sl.at(i+2);
        list_.append(mv);
        QColor color(sl.at(i+3).toInt(), sl.at(i+4).toInt(), sl.at(i+5).toInt(), sl.at(i+6).toInt());
        mv.color = color;
        mv.col = (int)Column::COLOR;
        mv.value="";
        list_.append(mv);
        i += query_size;
    }
    rows_ = rows;
    return rows;
}


void ColorModel::printValues(const QString& area) const
{
    qDebug() << "=================" << area << "(" << rows_ << ")=================";
    foreach(ModelValue item, list_) {
        QString tmp;
        tmp = QString("r:%1,c:%2,v:%3").arg(item.row).arg(item.col).arg(item.value);
        qDebug() << tmp;
    }
}

