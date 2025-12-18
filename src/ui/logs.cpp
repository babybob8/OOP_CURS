#include "logs.h"
#include "ui_logs.h"

Logs::Logs(QWidget *parent, QStringList logs, QString fname) :
    QWidget(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);
    ui->logsPlainTextEdit->clear();

    for (const QString& line : logs)
    {
        ui->logsPlainTextEdit->appendPlainText(line);
        ui->logsPathLabel->setText(fname);
    }
}

Logs::~Logs()
{
    delete ui;
}

void Logs::appendLog(const QString& formattedMessage)
{
    ui->logsPlainTextEdit->appendPlainText(formattedMessage);
}
