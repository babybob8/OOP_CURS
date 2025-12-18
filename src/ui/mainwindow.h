#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../share/parse.h"

#include <vector>
#include <map>

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionShow_Students_Table_triggered();

    void on_actionShow_Works_Table_triggered();

    void on_actionShow_Results_Table_triggered();

    void on_actionAbout_triggered();

    void on_actionShow_Logs_triggered();

    void addTable(const QString& filePath);

    void on_actionAdd_Students_triggered();

    void on_actionAdd_Works_triggered();

    void on_actionAdd_Results_triggered();

    void on_actionView_Resulting_Table_triggered();

private:
    Ui::MainWindow *ui;

    void loadSubWindow(QWidget *widget);

    std::map<TableType, QString> fileNames;
    QStringList logs;
    std::vector<std::vector<std::string>> students_table;
    std::vector<std::vector<std::string>> works_table;
    std::vector<std::vector<std::string>> results_table;
    std::vector<std::vector<std::string>> output_table;
};

#endif // MAINWINDOW_H
