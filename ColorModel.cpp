#include "ColorModel.h"
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
        // case Qt::EditRole:
        // {
        //     if(index.column() == (int)Column::REMOVE) {
        //         pb->setCheckable(true);
        //         pb->setChecked(true);
        //     }
        //     break;
        // }
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


void ColorModel::setComboBox(QModelIndex index, QString value)
{
    ModelValue mv = {index.row(), index.column(), value, Qt::gray};

    for( ModelListIterator it( list_ ); it.hasNext(); ) {
        auto myObj( it.next() );
        if( myObj.row == mv.row && myObj.col >= mv.col && myObj.col < (int)Column::COLOR) {
            it.remove();
            break;
        }
    }
    list_.append(mv);
}


void ColorModel::setColor(QModelIndex &index, QColor color)
{
    ModelValue mv = {index.row(), index.column(), "", color};
    for( ModelListIterator it( list_ ); it.hasNext(); ) {
        auto myObj( it.next() );
        if( myObj.row == mv.row && myObj.col == (int)Column::COLOR) {
            it.remove();
            break;
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


void ColorModel::removeAll()
{
    while(rows_ > 0) {
        removeRow(rows_ - 1);
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
    if(ret == false) {
        QMessageBox msgBox;
        msgBox.setText(QString("Saving colors to db failed"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    } else {
        emit oUpdated();
    }
}


int ColorModel::restore()
{
    ModelValue mv;
    const int query_size = 7;
    removeAll();
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QStringList sl = sqlInteface->getColorTable();
    int rows = sl.size() / query_size;
    int i = 0;
    QModelIndex nIndex;
    while(i < sl.size()) {
        int cur_row = i / query_size;
        mv.row = cur_row;
        mv.col = 0;
        mv.value = sl.at(i);
        list_.append(mv);
        nIndex = QAbstractItemModel::createIndex(cur_row, mv.col);
        setComboBox(nIndex, mv.value);
        emit dataChanged(nIndex, nIndex);
        mv.col = 1;
        mv.value = sl.at(i+1);
        list_.append(mv);
        nIndex = QAbstractItemModel::createIndex(cur_row, mv.col);
        setComboBox(nIndex, mv.value);
        emit dataChanged(nIndex, nIndex);
        mv.col = 2;
        mv.value = sl.at(i+2);
        list_.append(mv);
        nIndex = QAbstractItemModel::createIndex(cur_row, mv.col);
        setComboBox(nIndex, mv.value);
        emit dataChanged(nIndex, nIndex);
        QColor color(sl.at(i+3).toInt(), sl.at(i+4).toInt(), sl.at(i+5).toInt(), sl.at(i+6).toInt());
        mv.color = color;
        mv.col = (int)Column::COLOR;
        mv.value="";
        list_.append(mv);
        i += query_size;
    }
    rows_ = rows;
    setRowCount(rows);
    return rows;
}


void ColorModel::loadDefault()
{
    removeAll();
    list_.clear();
    ModelValue mv;
    int row = 0;
    fillColorRow(row++, "TPS", "TPS Tech1", "", "#1A5FB4");
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan", "", "#1A5FB4");
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev1", "", "#99C1F1");
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev2", "", "#62A0EA");
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev3", "", "#3584E4");
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan QA", "", "#613583");
    fillColorRow(row++, "TPS", "TPS Tech2", "", "#26A269");
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live", "", "#26A269");
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live Dev1", "", "#8FF0A4");
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live Dev2", "", "#33D17A");
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live QA", "", "#9141AC");
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-TRW", "", "#DC8ADD");
    fillColorRow(row++, "TPS", "TPS PLM Plan", "", "#865E3C");
    fillColorRow(row++, "TPS", "TPS PLM Live", "", "#CDAB8F");
    fillColorRow(row++, "TPS", "TPS Tech1 Production", "", "#F66151");
    fillColorRow(row++, "TPS", "TPS Tech1 Platform", "", "#48AEB5");
    fillColorRow(row++, "TPS", "TPS Tech1 CustomerCare", "", "#6CFD0C");
    fillColorRow(row++, "TPS", "TPS Bid", "", "#A51D2D");
    fillColorRow(row++, "TPS", "TPS Bid Documentation", "", "#A51D2D");
    fillColorRow(row++, "TPS", "TPS Tech1 SMiP", "", "#E5A50A");
    fillColorRow(row++, "TPS", "TPS Tech2 SMiP", "", "#F9F06B");
    fillColorRow(row++, "TPS", "TPS PLM", "", "#C64600");
    fillColorRow(row++, "TPS", "TPS PLM Plan", "", "#E66100");
    fillColorRow(row++, "TPS", "TPS PLM Live", "", "#FFBE6F");
    fillColorRow(row++, "TPS", "TPS PLM TRW", "", "#F708FB");
    fillColorRow(row++, "TPS", "TPS PLM Algorithms", "", "#FF7800");
    fillColorRow(row++, "TPS", "TPS Tech1 PM", "", "#F5C211");
    fillColorRow(row++, "TPS", "TPS Tech2 PM", "", "#F8E45C");
    fillColorRow(row++, "TPS", "TPS QM Excellence", "", "#04FCED");
    fillColorRow(row++, "TPS", "TPS R&D Excellence", "", "#04FCED");
}


void ColorModel::fillColorRow(int row, QString dep, QString team, QString comp, QColor color)
{
    addRow();
    ModelValue mv;
    QModelIndex nIndex;
    mv.row = row;

    mv.col = (int)Columns::DEPARTMENT;
    mv.value = dep;
    nIndex = QAbstractItemModel::createIndex(mv.row, mv.col);
    setComboBox(nIndex, mv.value);
    list_.append(mv);
    emit dataChanged(nIndex, nIndex);

    mv.col = (int)Columns::TEAM;
    mv.value = team;
    nIndex = QAbstractItemModel::createIndex(mv.row, mv.col);
    setComboBox(nIndex, mv.value);
    list_.append(mv);
    emit dataChanged(nIndex, nIndex);

    mv.col = (int)Columns::COMPONENT;
    mv.value = comp;
    nIndex = QAbstractItemModel::createIndex(mv.row, mv.col);
    setComboBox(nIndex, mv.value);
    list_.append(mv);
    emit dataChanged(nIndex, nIndex);

    mv.col = (int)Columns::COLOR;
    mv.color = color;
    nIndex = QAbstractItemModel::createIndex(mv.row, mv.col);
    list_.append(mv);
    emit dataChanged(nIndex, nIndex);
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
