#include "ColorView.h"
#include <QHeaderView>

ColorView::ColorView(QWidget *parent) : QTableView(parent)
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
    QFont fnt  ( "Helvetica", 11, QFont::Normal );
    this->setFont(fnt);
}


void ColorView::insertRow()
{

}


void ColorView::removeRow()
{

}
