#include "students.h"
#include "ui_students.h"
#include "restable.h"

Students::Students(QWidget *parent, std::vector<std::vector<std::string>> table, QString fname) :
    QWidget(parent),
    ui(new Ui::Students)
{
    ui->setupUi(this);
    ui->studentsPathPabel->setText(fname);
    fillTable(ui->studentsTableWidget, table);
}

Students::~Students()
{
    delete ui;
}
