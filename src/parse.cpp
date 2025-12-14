#include "parse.h"

#include <QString>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <exception>

std::vector<std::string> split_csv_line(const std::string& line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ',')) {
        result.push_back(cell);
    }

    return result;
}

std::vector<std::vector<std::string>> c_parse_file(const QString& filename)
{
    std::ifstream file(filename.toStdString());
//    if (file.bad()) throw std::runtime_error("File is considered bad.");
    if (!file.is_open()) throw std::runtime_error("Error opening file.");

    std::vector<std::vector<std::string>> file_content;
    std::string line;
    while (std::getline(file, line))
    {
        file_content.push_back(split_csv_line(line));
    }

    file.close();
    return file_content;
}

TableType get_TableType(const std::vector<std::string>& head)
{
    bool isStud = false;
    bool isWork = false;
    bool isResult = false;

    bool hasNameStud = std::find(head.begin(), head.end(), "name") != head.end();
    bool hasFName = std::find(head.begin(), head.end(), "fName") != head.end();
    if (hasNameStud || hasFName)
    {
        isStud = true;
    }

    bool hasNameWork = std::find(head.begin(), head.end(), "work") != head.end();
    bool hasMax = std::find(head.begin(), head.end(), "max_score") != head.end();
    bool hasMustToDo = std::find(head.begin(), head.end(), "mandatory") != head.end();
    bool hasSum = std::find(head.begin(), head.end(), "add_flag") != head.end();
    if (hasNameWork && hasMax && hasMustToDo && hasSum)
    {
        isWork = true;
    }

    bool hasFio = std::find(head.begin(), head.end(), "student") != head.end();
    bool hasWork = std::find(head.begin(), head.end(), "work") != head.end();
    bool hasEstimaty = std::find(head.begin(), head.end(), "score") != head.end();
    if (hasFio && hasWork && hasEstimaty)
    {
        isResult = true;
    }

    if (isStud && !isWork && !isResult) return TableType::Student;
    if (!isStud && isWork && !isResult) return TableType::Work;
    if (!isStud && !isWork && isResult) return TableType::Result;
    return TableType::Err;
}
