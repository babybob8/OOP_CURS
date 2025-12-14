#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parse.h"
#include "config.h"

#include <QMainWindow>
#include <QString>
#include <QFile>

#include <vector>
#include <map>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void appendLog(const QString& message);

private:
    void loadInitialLogs();

    Ui::MainWindow *ui;
    std::map<TableType, std::vector<std::vector<std::string>>> m_tables;
    QString m_LogFilePath = QString::fromStdString(Config::LogPath.data());

};
#endif // MAINWINDOW_H
