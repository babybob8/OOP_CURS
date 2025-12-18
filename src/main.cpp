#include "ui/mainwindow.h"
#include "share/config.h"
#include "share/log.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList args = a.arguments();
    if (args.contains("-c"))
    {
        Config::CheckConditions = true;
        log_info("Condition checks enabled via special key.");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
