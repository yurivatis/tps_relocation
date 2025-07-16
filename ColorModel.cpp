#include "ColorModel.h"
#include "constants.h"
#include "SQLite.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QColor>

ColorModel::ColorModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}


int ColorModel::rowCount(const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : data_.size();
}


int ColorModel::columnCount(const QModelIndex & parent) const
{
    return parent.isValid() ? 0 : (int)Column::TOTAL_COLUMNS;
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


QVariant ColorModel::data( const QModelIndex &index, int role ) const
{
    if( index.row() >= data_.size() )
        return QVariant();

    switch( (Column)index.column() )
    {
    case Column::DEPARTMENT:
        if( role == Qt::DisplayRole )
            return data_[ index.row() ].department_;
        break;
    case Column::TEAM:
        if( role == Qt::DisplayRole )
            return data_[ index.row() ].team_;
        break;
    case Column::COMPONENT:
        if( role == Qt::DisplayRole )
            return data_[ index.row() ].component_;
        break;
    case Column::COLOR:
        if( role == Qt::BackgroundRole )
            return data_[ index.row() ].color_;
        break;
    default:
        return QVariant();
    }
    return QVariant();
}


bool ColorModel::setData( const QModelIndex &index, const QVariant &data, int role )
{
    if( index.row() >= data_.size() )
        return false;

    switch( (Column)index.column() ) {
    case Column::DEPARTMENT:
        if( role == Qt::DisplayRole ) {
            const bool changed = data_[ index.row() ].department_ != data.toString();
            data_[ index.row() ].department_ = data.toString();
            if( changed )
            {
                // Update team immediately after department change
                auto sibling = index.sibling(index.row(), index.column() + 1);
                setData( sibling, firstTeamOfDepartment( data.toString() ), Qt::DisplayRole );
                emit dataChanged( index, sibling, { Qt::DisplayRole } );
            } else {
                emit dataChanged( index, index, { Qt::DisplayRole } );
            }
            return true;
        }
        break;
    case Column::TEAM:
        if( role == Qt::DisplayRole ) {
            data_[ index.row() ].team_ = data.toString();
            emit dataChanged( index, index, { Qt::DisplayRole } );
            return true;
        }
        break;
    case Column::COMPONENT:
        if( role == Qt::DisplayRole ) {
            data_[ index.row() ].component_ = data.toString();
            emit dataChanged( index, index, { Qt::DisplayRole } );
            return true;
        }
        break;
    case Column::REMOVE:
        if( role == Qt::DisplayRole ) {
            return "Remove Row";
        }
        break;
    case Column::COLOR:
        if( role == Qt::BackgroundRole ) {
            data_[ index.row() ].color_ = data.value< QColor >();
            emit dataChanged( index, index, { Qt::BackgroundRole } );
            return true;
        }
        break;
    default:
        return false;
    }
    return false;
}


void ColorModel::addRow()
{
    beginInsertRows( QModelIndex(), data_.size(), data_.size() );
    data_.push_back( {} );
    endInsertRows();
}


void ColorModel::removeRow(int row)
{
    removeRows( row, 1, QModelIndex() );
}


void ColorModel::removeAll()
{
    removeRows( 0, data_.size(), QModelIndex() );
}


bool ColorModel::removeRows( int row, int count, const QModelIndex &parent )
{
    if( row < 0 || row >= data_.size() )
        return false;

    beginRemoveRows( parent, row, row + count - 1 );
    data_.remove( row, count );
    endRemoveRows();

    return true;
}


void ColorModel::save()
{
    bool ret = true;
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    sqlInteface->clearColorTable();

    for(int i = 0; i < data_.size(); i++) {
        const auto &data = data_[ i ];
        if(sqlInteface->writeColor(data.department_, data.team_, data.component_, data.color_) == false)
        {
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
    const int query_size = 7;
    removeAll();
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QStringList sl = sqlInteface->getColorTable();
    int rows = sl.size() / query_size;
    int i = 0;

    beginInsertRows( QModelIndex(), 0, rows - 1 );
    while(i < sl.size()) {
        RowData data;
        data.department_ = sl.at(i);
        data.team_ = sl.at(i+1);
        data.component_ = sl.at(i+2);
        QColor color(sl.at(i+3).toInt(), sl.at(i+4).toInt(), sl.at(i+5).toInt(), sl.at(i+6).toInt());
        data.color_ = color;

        data_.push_back( data );
        i += query_size;
    }
    endInsertRows();
    return rows;
}


void ColorModel::loadDefault()
{
    removeAll();

    QVector< RowData > defaultValue;
    defaultValue.push_back( { "TPS", "TPS Tech1", "", QColor(0x1A, 0x5F, 0xB4) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 R&D-Plan", "", QColor(0x1A, 0x5F, 0xB4) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 R&D-Plan Dev1", "", QColor(0x99, 0xC1, 0xF1) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 R&D-Plan Dev2", "", QColor(0x62, 0xA0, 0xEA) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 R&D-Plan Dev3", "", QColor(0x35, 0x84, 0xE4) });
    defaultValue.push_back( { "TPS", "TPS Tech1 R&D-Plan QA", "", QColor(0x83, 0x49, 0xFF) } );
    defaultValue.push_back( { "TPS", "TPS Tech2", "", QColor(0x26, 0xA2, 0x69) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live", "", QColor(0x26, 0xA2, 0x69) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev1", "", QColor(0x26, 0xA2, 0x69) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev1", "RTC", QColor(0x33, 0xD1, 0x7A) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev1", "D&P", QColor(0x43, 0xA0, 0x47) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev1", "RISE", QColor(0x8F, 0xF0, 0xA4) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev2", "", QColor(0x42, 0xBC, 0x20) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live Dev2", "RISE", QColor(0x8F, 0xF0, 0xA4) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-Live QA", "", QColor(0x48, 0xAE, 0xB5) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 R&D-TRW", "", QColor(0xDC, 0x8A, 0xDD) } );
    defaultValue.push_back( { "TPS", "TPS PLM Plan", "", QColor(0x86, 0x5E, 0x3C) } );
    defaultValue.push_back( { "TPS", "TPS PLM Live", "", QColor(0xCD, 0xAB, 0x8F) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 Production", "", QColor(0xF6, 0x61, 0x51) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 Platform, Deployment and Hosting", "", QColor(0xA5, 0x1D, 0x2D) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 CustomerCare", "", QColor(0x6C, 0xFD, 0x0C) } );
    defaultValue.push_back( { "TPS", "TPS Bid", "", QColor(0x54, 0x74, 0x74) } );
    defaultValue.push_back( { "TPS", "TPS Sales Logistics", "", QColor(0x54, 0x74, 0x74) } );
    defaultValue.push_back( { "TPS", "TPS Bid Documentation", "", QColor(0xC9, 0xDA, 0xB0) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 SMiP", "", QColor(0xE5, 0xA5, 0x0A) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 SMiP", "", QColor(0xF9, 0xF0, 0x6B) } );
    defaultValue.push_back( { "TPS", "TPS PLM", "", QColor(0xC6, 0x46, 0x00) } );
    defaultValue.push_back( { "TPS", "TPS PLM Plan", "", QColor(0xE6, 0x61, 0x00) } );
    defaultValue.push_back( { "TPS", "TPS PLM Live", "", QColor(0xFF, 0xBE, 0x6F) } );
    defaultValue.push_back( { "TPS", "TPS PLM TRW", "",  QColor(0xF7, 0x08, 0xFB) } );
    defaultValue.push_back( { "TPS", "TPS Tech1 PM", "", QColor(0xF5, 0xA7, 0x98) } );
    defaultValue.push_back( { "TPS", "TPS Tech2 PM", "", QColor(0xD2, 0xF8, 0xE0) });
    defaultValue.push_back( { "TPS", "TPS Excellence", "", QColor(0x04, 0xFC, 0xED) } );
    defaultValue.push_back( { "HACON Management", "Managing Directors", "", QColor(0xFF, 0x00, 0x00) } );
    defaultValue.push_back( { "HACON Functions", "Management Assistance", "", QColor(0xFF, 0x00, 0x7F) } );
    defaultValue.push_back( { "HACON Management", "TPS Sales Logistics", "", QColor(0xAA, 0xAA, 0x7F) } );
    defaultValue.push_back( { "HACON Management", "TPS Techn.Advisor", "", QColor(0xAA, 0xAA, 0x7F) } );
    beginInsertRows( QModelIndex(), 0, defaultValue.size() - 1 );
    data_ = defaultValue;
    endInsertRows();
}


Qt::ItemFlags ColorModel::flags( const QModelIndex & /*index*/) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}


QString ColorModel::firstTeamOfDepartment( const QString &department ) const
{
    SqlInterface *sqlInteface = SqlInterface::getInstance();
    QStringList list = sqlInteface->teams( department );
    return list.empty() ? "" : list.front();
}
