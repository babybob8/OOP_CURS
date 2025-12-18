#ifndef WORK_H
#define WORK_H

#include <QWidget>

namespace Ui {
class Work;
}

class Work : public QWidget
{
    Q_OBJECT

public:
    explicit Work(QWidget *parent = nullptr,
                  std::vector<std::vector<std::string>> table = {},
                  QString fname = "");
    ~Work();

private:
    Ui::Work *ui;
};

#endif // WORK_H
