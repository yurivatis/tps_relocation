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
    fillColorRow(row++, "TPS", "TPS Tech1", "", QColor(0x1A, 0x5F, 0xB4));
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan", "", QColor(0x1A, 0x5F, 0xB4));
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev1", "", QColor(0x99, 0xC1, 0xF1));
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev2", "", QColor(0x62, 0xA0, 0xEA));
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan Dev3", "", QColor(0x35, 0x84, 0xE4));
    fillColorRow(row++, "TPS", "TPS Tech1 R&D-Plan QA", "", QColor(0x61, 0x35, 0x83));
    fillColorRow(row++, "TPS", "TPS Tech2", "", QColor(0x26, 0xA2, 0x69));
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live", "", QColor(0x33, 0xD1, 0x7A));
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live Dev1", "", QColor(0x8F, 0xF0, 0xA4));
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live Dev2", "", QColor(0x33, 0xD1, 0x7A));
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-Live QA", "", QColor(0x91, 0x41, 0xAC));
    fillColorRow(row++, "TPS", "TPS Tech2 R&D-TRW", "", QColor(0xDC, 0x8A, 0xDD));
    fillColorRow(row++, "TPS", "TPS PLM Plan", "", QColor(0x86, 0x5E, 0x3C));
    fillColorRow(row++, "TPS", "TPS PLM Live", "", QColor(0xCD, 0xAB, 0x8F));
    fillColorRow(row++, "TPS", "TPS Tech1 Production", "", QColor(0xF6, 0x61, 0x51));
    fillColorRow(row++, "TPS", "TPS Tech1 Platform", "", QColor(0x48, 0xAE, 0xB5));
    fillColorRow(row++, "TPS", "TPS Tech1 CustomerCare", "", QColor(0x6C, 0xFD, 0x0C));
    fillColorRow(row++, "TPS", "TPS Bid", "", QColor(0xA5, 0x1D, 0x2D));
    fillColorRow(row++, "TPS", "TPS Bid Documentation", "", QColor(0xA5, 0x1D, 0x2D));
    fillColorRow(row++, "TPS", "TPS Tech1 SMiP", "", QColor(0xE5, 0xA5, 0x0A));
    fillColorRow(row++, "TPS", "TPS Tech2 SMiP", "", QColor(0xF9, 0xF0, 0x6B));
    fillColorRow(row++, "TPS", "TPS PLM", "", QColor(0xC6, 0x46, 0x00));
    fillColorRow(row++, "TPS", "TPS PLM Plan", "", QColor(0xE6, 0x61, 0x00));
    fillColorRow(row++, "TPS", "TPS PLM Live", "", QColor(0xFF, 0xBE, 0x6F));
    fillColorRow(row++, "TPS", "TPS PLM TRW", "",  QColor(0xF7, 0x08, 0xFB));
    fillColorRow(row++, "TPS", "TPS PLM Algorithms", "", QColor(0xFF, 0x78, 0x00));
    fillColorRow(row++, "TPS", "TPS Tech1 PM", "", QColor(0xF5, 0xA7, 0x98));
    fillColorRow(row++, "TPS", "TPS Tech2 PM", "", QColor(0xD2, 0xF8, 0xE0));
    fillColorRow(row++, "TPS", "TPS QM Excellence", "", QColor(0x04, 0xFC, 0xED));
    fillColorRow(row++, "TPS", "TPS R&D Excellence", "", QColor(0x04, 0xFC, 0xED));
}

//#f5a798
//#d2f8e0
//fillColorRow(row++, "TPS", "TPS PLM Plan", "", QColor(0xF5, 0xA7, 0x98));
//fillColorRow(row++, "TPS", "TPS PLM Live", "", QColor(0xD2, 0xF8, 0xE0));

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
