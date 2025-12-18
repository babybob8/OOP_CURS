#ifndef STUDENTS_H
#define STUDENTS_H

#include <QWidget>

namespace Ui {
class Students;
}

class Students : public QWidget
{
    Q_OBJECT

public:
    explicit Students(QWidget *parent = nullptr,
                      std::vector<std::vector<std::string>> table = {},
                      QString fname = "");
    ~Students();

private:
    Ui::Students *ui;
};

#endif // STUDENTS_H
