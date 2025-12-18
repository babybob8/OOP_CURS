#include "restable.h"
#include "ui_restable.h"

void fillTable(QTableWidget *table, const std::vector<std::vector<std::string>> &data)
{
    if (data.empty())
    {
        table->clearContents();
        table->setRowCount(0);
        table->setColumnCount(0);
        return;
    }

    size_t rows = data.size();
    size_t cols = data[0].size();

    table->setRowCount(static_cast<int>(rows));
    table->setColumnCount(static_cast<int>(cols));

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            if (j < data[i].size())
            {
                table->setItem(static_cast<int>(i), static_cast<int>(j),
                               new QTableWidgetItem(QString::fromStdString(data[i][j])));
            }
            else
            {
                table->setItem(static_cast<int>(i), static_cast<int>(j), new QTableWidgetItem(""));
            }
        }
    }
}

ResTable::ResTable(QWidget *parent, std::vector<std::vector<std::string>> table, QString fname) :
    QWidget(parent),
    ui(new Ui::ResTable)
{
    ui->setupUi(this);
    ui->outputPathLabel->setText(fname);
    fillTable(ui->outputTableWidget, table);
}

ResTable::~ResTable()
{
    delete ui;
}
