#ifndef RESTABLE_H
#define RESTABLE_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class ResTable;
}

class ResTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResTable(QWidget *parent = nullptr);
    ~ResTable();

    QTableWidget* getTableWidget();

private:
    Ui::ResTable *ui;
};

#endif // RESTABLE_H
