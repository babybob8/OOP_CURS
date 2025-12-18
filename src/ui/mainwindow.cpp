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

#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    setCentralWidget(ui->mdiArea);
    connect(&logger::instance(), &logger::newLog,
            this, [this](const QString& formattedMessage) {
        logs.append(formattedMessage);
    });

    connect(ui->menuWindows, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionShow_Students_Table_triggered()
{
    if (studentsSubWindow && studentsSubWindow->widget())
    {
        ui->mdiArea->setActiveSubWindow(studentsSubWindow);
        return;
    }
    Students* studentsWidget = new Students(this, students_table, fileNames[TableType::Student]);
    studentsSubWindow = ui->mdiArea->addSubWindow(studentsWidget);
    studentsSubWindow->setWindowTitle("Students Widget");
    studentsSubWindow->setWindowIcon(studentsWidget->windowIcon());
    studentsSubWindow->show();

    connect(studentsSubWindow, &QObject::destroyed, this, [this]() {
        studentsSubWindow = nullptr;
    });
}


void MainWindow::on_actionShow_Works_Table_triggered()
{
    if (worksSubWindow && worksSubWindow->widget())
    {
        ui->mdiArea->setActiveSubWindow(worksSubWindow);
        return;
    }
    Work* worksWidget = new Work(this, works_table, fileNames[TableType::Work]);
    worksSubWindow = ui->mdiArea->addSubWindow(worksWidget);
    worksSubWindow->setWindowTitle("Works Widget");
    worksSubWindow->setWindowIcon(worksWidget->windowIcon());
    worksSubWindow->show();

    connect(worksSubWindow, &QObject::destroyed, this, [this]() {
        worksSubWindow = nullptr;
    });
}


void MainWindow::on_actionShow_Results_Table_triggered()
{
    if (resultsSubWindow && resultsSubWindow->widget())
    {
        ui->mdiArea->setActiveSubWindow(resultsSubWindow);
        return;
    }
    Results* resultsWidget = new Results(this, results_table, fileNames[TableType::Result]);
    resultsSubWindow = ui->mdiArea->addSubWindow(resultsWidget);
    resultsSubWindow->setWindowTitle("Results Widget");
    resultsSubWindow->setWindowIcon(resultsWidget->windowIcon());
    resultsSubWindow->show();

    connect(resultsSubWindow, &QObject::destroyed, this, [this]() {
        resultsSubWindow = nullptr;
    });
}

void MainWindow::on_actionAbout_triggered()
{
    About about(this);
    about.setWindowTitle(tr("About"));
    about.setWindowModality(Qt::WindowModal);
    about.exec();
}

void MainWindow::on_actionShow_Logs_triggered()
{
    if (logsSubWindow && logsSubWindow->widget())
    {
        ui->mdiArea->setActiveSubWindow(logsSubWindow);
        return;
    }

    Logs *logsWidget = new Logs(this, logs, QString::fromStdString(Config::LogPath.data()));
    connect(&logger::instance(), &logger::newLog,
            logsWidget, &Logs::appendLog);

    logsSubWindow = ui->mdiArea->addSubWindow(logsWidget);
    logsSubWindow->setWindowTitle("Logs Widget");
    logsSubWindow->setWindowIcon(logsWidget->windowIcon());
    logsSubWindow->show();

    connect(logsSubWindow, &QObject::destroyed, this, [this]() {
        logsSubWindow = nullptr;
    });
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
    if (resTableSubWindow && resTableSubWindow->widget())
    {
        ui->mdiArea->setActiveSubWindow(resTableSubWindow);
        return;
    }
    ResTable* resTableWidget = new ResTable(this, output_table, QString::fromStdString(Config::OutPath.data()));
    resTableSubWindow = ui->mdiArea->addSubWindow(resTableWidget);
    resTableSubWindow->setWindowTitle("Output Table Widget");
    resTableSubWindow->setWindowIcon(resTableWidget->windowIcon());
    resTableSubWindow->show();

    connect(resTableSubWindow, &QObject::destroyed, this, [this]() {
        resTableSubWindow = nullptr;
    });
}

void MainWindow::updateWindowMenu()
{
    ui->menuWindows->clear();

    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); ++i)
    {
        QMdiSubWindow *subWindow = windows.at(i);
        QString title = subWindow->windowTitle().isEmpty() ? tr("Untitled") : subWindow->windowTitle();
        QString text = QString("%1 - %2").arg(i + 1).arg(title);

        QAction *action = ui->menuWindows->addAction(text);
        action->setCheckable(true);
        action->setChecked(subWindow == ui->mdiArea->activeSubWindow());

        connect(action, &QAction::triggered, this, [this, subWindow](){
            if (subWindow)
            {
                ui->mdiArea->setActiveSubWindow(subWindow);
                subWindow->showNormal();
                subWindow->raise();
                subWindow->setFocus();
            }
        });
    }
}
