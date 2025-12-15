#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>

#include <QString>

enum class TableType
{
    Student,
    Work,
    Result,
    Err
};

std::vector<std::string> split_csv_line(const std::string& line);
std::vector<std::vector<std::string>> c_parse_file(const QString& filename);
std::vector<std::vector<std::string>> c_parse_file(const std::string& filename);
TableType get_TableType(const std::vector<std::string>& head);
void write_csv(const std::vector<std::vector<std::string>>& table, const std::string& f_name);

#endif // PARSE_H
