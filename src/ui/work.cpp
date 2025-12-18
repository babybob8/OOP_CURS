#include "work.h"
#include "ui_work.h"
#include "restable.h"

Work::Work(QWidget *parent, std::vector<std::vector<std::string>> table, QString fname) :
    QWidget(parent),
    ui(new Ui::Work)
{
    ui->setupUi(this);
    ui->workPathLabel->setText(fname);
    fillTable(ui->workTableWidget, table);
}

Work::~Work()
{
    delete ui;
}
