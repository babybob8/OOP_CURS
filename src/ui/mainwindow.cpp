#include "mainwindow.h"
#include "about.h"
#include "logs.h"
#include "restable.h"
#include "results.h"
#include "students.h"
#include "work.h"
#include "about.h"
#include "ui_mainwindow.h"

#include "../share/parse.h"
#include "../share/log.h"
#include "../share/book.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    connect(&logger::instance(), &logger::newLog,
            this, [this](const QString& formattedMessage) {
        logs.append(formattedMessage);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionShow_Students_Table_triggered()
{
    loadSubWindow(new Students(this, students_table, fileNames[TableType::Student]));
}


void MainWindow::on_actionShow_Works_Table_triggered()
{
    loadSubWindow(new Work(this, works_table, fileNames[TableType::Work]));
}


void MainWindow::on_actionShow_Results_Table_triggered()
{
    loadSubWindow(new Results(this, results_table, fileNames[TableType::Result]));
}

void MainWindow::on_actionAbout_triggered()
{
    loadSubWindow(new About(this));
}

void MainWindow::on_actionShow_Logs_triggered()
{

    Logs *logsWindow = new Logs(this, logs, QString::fromStdString(Config::LogPath.data()));

    connect(&logger::instance(), &logger::newLog,
            logsWindow, &Logs::appendLog);

    loadSubWindow(logsWindow);
}

void MainWindow::loadSubWindow(QWidget *widget)
{
    auto window = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle(widget->windowTitle());
    window->setWindowIcon(widget->windowIcon());
    window->show();
}

void MainWindow::addTable(const QString& filePath)
{
    try
    {
        std::vector<std::vector<std::string>> f_contents = c_parse_file(filePath);
        if (is_Header(f_contents.front().front()))
        {
            f_contents[0][0].erase(0, 1);
            TableType tabletype = get_TableType(f_contents.front());
            switch (tabletype)
            {
            case TableType::Err:
            {
                log_error("Unknown table type in file [ " + filePath.toStdString() + " ]");
                return;
            }
            case TableType::Student:
            {
                students_table = f_contents;
                fileNames[TableType::Student] = filePath;
                log_info("Added Students table [ " + filePath.toStdString() + " ]");
                ui->actionShow_Students_Table->setEnabled(true);
                break;
            }
            case TableType::Work:
            {
                works_table = f_contents;
                fileNames[TableType::Work] = filePath;
                log_info("Added Works table [ " + filePath.toStdString() + " ]");
                ui->actionShow_Works_Table->setEnabled(true);
                break;
            }
            case TableType::Result:
            {
                results_table = f_contents;
                fileNames[TableType::Result] = filePath;
                log_info("Added Results table [ " + filePath.toStdString() + " ]");
                ui->actionShow_Results_Table->setEnabled(true);
                break;
            }
            }
        }
        else
        {
            log_error("File [ " + filePath.toStdString() + " ] has no header.");
        }
    }
    catch (const std::exception& e)
    {
        log_error(e.what());
    }
}

void MainWindow::on_actionAdd_Students_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
            this,
            "Choose csv table with Student's names",
            "../../data",
            "Csv Tables (*.csv)"
        );
    if (!filePath.isEmpty())
    {
        addTable(filePath);
        if (!students_table.empty() && !works_table.empty() && !results_table.empty())
        {
            output_table = generateCurrentResults(students_table, works_table, results_table);
            ui->actionView_Resulting_Table->setEnabled(true);
            write_csv(output_table, Config::OutPath.data());
            log_info(std::string("Output file generated at [ ") + Config::OutPath.data() + " ]." );
        }
    }
}

void MainWindow::on_actionAdd_Works_triggered()
{
    on_actionAdd_Students_triggered();
}

void MainWindow::on_actionAdd_Results_triggered()
{
    on_actionAdd_Students_triggered();
}

void MainWindow::on_actionView_Resulting_Table_triggered()
{
    loadSubWindow(new ResTable(this, output_table, QString::fromStdString(Config::OutPath.data())));
}

