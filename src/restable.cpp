#include "restable.h"
#include "ui_restable.h"

ResTable::ResTable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResTable)
{
    ui->setupUi(this);
    ui->resTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ResTable::~ResTable()
{
    delete ui;
}

QTableWidget* ResTable::getTableWidget()
{
    return ui->resTableWidget;
}
