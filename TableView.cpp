#include "TableView.h"
#include <QHeaderView>

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    QHeaderView *hHeaderView = horizontalHeader();
    hHeaderView->setSectionsClickable(true);
    hHeaderView->setStretchLastSection(true);
    hHeaderView->setHighlightSections(false);

    QHeaderView *vHeaderView = verticalHeader();
    vHeaderView->setSectionsClickable(true);
    vHeaderView->setStretchLastSection(false);
    vHeaderView->setHighlightSections(false);
    vHeaderView->show();

    this->setSelectionMode(QAbstractItemView::NoSelection);
    QFont fnt("Helvetica", 11, QFont::Normal );
    this->setFont(fnt);

    connect( this, &TableView::clicked, this, &TableView::cellClicked );
}

void TableView::cellClicked( const QModelIndex &index )
{
    // Call of edit() to trigger the editor of the clicked ItemDelegate
    if( index.isValid() )
        edit( index );
}

