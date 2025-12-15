#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parse.h"
#include "book.h"
#include "restable.h"
#include "logger.h"
#include "log.h"

#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void critical_message_handler(const QString& msg)
{
    log_critical(msg.toStdString());
    QMessageBox::critical(nullptr, "Error", msg);
}

void c_ui_styling(Ui::MainWindow* ui)
{
//    ui->stud_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stud_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->work_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->res_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    log_info("Ui Styled.");
}

void c_update_table(QTableWidget* table, const std::vector<std::vector<std::string>>& content)
{
    unsigned rows = content.size();
    unsigned cols = content[0].size();

    table->setRowCount(rows - 1);
    table->setColumnCount(cols);

    QStringList header;

    for (const auto& str : content[0])
    {
        header.append(QString::fromStdString(str));
    }

    table->setHorizontalHeaderLabels(header);

    for (unsigned row = 1; row < rows; ++row)
    {
        for (unsigned col = 0; col < cols; ++col)
        {
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(content[row][col]));
            table->setItem(row-1, col, item);
        }
    }
    std::string msg = "Table [" + table->objectName().toStdString() + "] got updated.";
    log_info(msg);
}

void c_compile_res(QTableWidget* table, std::map<TableType, std::vector<std::vector<std::string>>>& tables)
{
    auto stud_table  = tables[TableType::Student];
    auto work_table  = tables[TableType::Work];
    auto result_table  = tables[TableType::Result];
    auto comiled_table = generateCurrentResults(stud_table, work_table, result_table);

    log_info("Resulting table compiled.");

    try
    {
        write_csv(comiled_table, Config::OutPath.data());
    }
    catch (const std::exception& e)
    {
        std::string msg = "Results were not written in +" + std::string(Config::OutPath.data())
                + ". Error: " + std::string(e.what());
        log_error(msg);
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(msg));
    }

    c_update_table(table, comiled_table);
}

bool c_add_table(Ui::MainWindow* ui, QString file, std::map<TableType, std::vector<std::vector<std::string>>>& tables)
{
    std::vector<std::vector<std::string>> fcontent = c_parse_file(file);
    std::vector<std::string> fheader = fcontent[0];
    TableType ftype = get_TableType(fheader);

    switch (ftype)
    {
    case TableType::Student:
    {
        c_update_table(ui->stud_table, fcontent);
        tables[TableType::Student] = fcontent;
        break;
    }
    case TableType::Work:
    {
        c_update_table(ui->work_table, fcontent);
        tables[TableType::Work] = fcontent;
        break;
    }
    case TableType::Result:
    {
        c_update_table(ui->res_table, fcontent);
        tables[TableType::Result] = fcontent;
        break;
    }
    case TableType::Err:
        throw std::runtime_error("Unknown table type.");
    }

    auto has_student = tables.find(TableType::Student);
    auto has_work = tables.find(TableType::Work);
    auto has_result = tables.find(TableType::Result);

    if (has_student != tables.end() && has_work != tables.end() && has_result != tables.end())
    {
        return true;
    }

    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    logger::setErrorHandler([](const std::string& err)
    {
        critical_message_handler(QString::fromStdString(err));
    });
    log_info("Application started.");
    log_debug("In Debug Mode.");

    ui->setupUi(this);
    c_ui_styling(ui);

    connect(ui->actionCompile, &QAction::triggered, this, [&]()
    {
        ResTable* resWindow = new ResTable();
        resWindow->show();
        try
        {
            c_compile_res(resWindow->getTableWidget(), m_tables);
        }
        catch (const std::exception& e)
        {
            critical_message_handler(e.what());
        }
    });

    connect(ui->actionLoad_Students, &QAction::triggered, this, [&]()
    {
        QString file = QFileDialog::getOpenFileName(
            this,
            tr("Load table"),
            "",
            tr("All files (*.*);;Text files (*.txt);;CSV files (*.csv)")
        );
        if (!file.isEmpty())
        {
            try
            {
                if(c_add_table(ui, file, m_tables))
                    ui->actionCompile->setEnabled(true);
            }
            catch (const std::exception& e)
            {
                critical_message_handler(e.what());
            }
        }
    });

    connect(ui->actionLoad_Work, &QAction::triggered, this, [&]()
    {
        QString file = QFileDialog::getOpenFileName(
            this,
            tr("Load table"),
            "",
            tr("All files (*.*);;Text files (*.txt);;CSV files (*.csv)")
        );
        if (!file.isEmpty())
        {
            try
            {
                if(c_add_table(ui, file, m_tables))
                    ui->actionCompile->setEnabled(true);
            }
            catch (const std::exception& e)
            {
                critical_message_handler(e.what());
            }
        }
    });

    connect(ui->actionLoad_Results, &QAction::triggered, this, [&]()
    {
        QString file = QFileDialog::getOpenFileName(
            this,
            tr("Load table"),
            "",
            tr("All files (*.*);;Text files (*.txt);;CSV files (*.csv)")
        );
        if (!file.isEmpty())
        {
            try
            {
                if(c_add_table(ui, file, m_tables))
                    ui->actionCompile->setEnabled(true);
            }
            catch (const std::exception& e)
            {
                critical_message_handler(e.what());
            }
        }
    });

    loadInitialLogs();

    connect(&logger::instance(), &logger::newLog, this, &MainWindow::appendLog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendLog(const QString& message)
{
    ui->logsTextEdit->appendPlainText(message);
    ui->logsTextEdit->ensureCursorVisible();
}

void MainWindow::loadInitialLogs()
{
    QFile file(m_LogFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString content = in.readAll();
        ui->logsTextEdit->setPlainText(content);
        ui->logsTextEdit->moveCursor(QTextCursor::End);
        file.close();
    }
    else
    {
        critical_message_handler("Failed to open log file for initial reading");
    }
}
