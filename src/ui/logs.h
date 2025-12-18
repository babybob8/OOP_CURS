#ifndef LOGS_H
#define LOGS_H

#include <QWidget>

namespace Ui {
class Logs;
}

class Logs : public QWidget
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr, QStringList logs = {}, QString fname = "");
    ~Logs();

public slots:
    void appendLog(const QString& formattedMessage);

private:
    Ui::Logs *ui;
};

#endif // LOGS_H
