#ifndef RESTABLE_H
#define RESTABLE_H

#include <vector>
#include <map>
#include <string>

#include <QWidget>
#include <QTableWidget>
#include <QString>

void fillTable(QTableWidget *table, const std::vector<std::vector<std::string>> &data);

namespace Ui {
class ResTable;
}

class ResTable : public QWidget
{
    Q_OBJECT

public:
    explicit ResTable(QWidget *parent = nullptr,
                      std::vector<std::vector<std::string>> table = {},
                      QString fname = "");
    ~ResTable();

private:
    Ui::ResTable *ui;
};

#endif // RESTABLE_H
