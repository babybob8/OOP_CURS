#include "results.h"
#include "ui_results.h"
#include "restable.h"

Results::Results(QWidget *parent, std::vector<std::vector<std::string>> table, QString fname) :
    QWidget(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
    ui->resultsPathLabel->setText(fname);
    fillTable(ui->resultsTableWidget, table);
}

Results::~Results()
{
    delete ui;
}
