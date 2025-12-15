#include "parse.h"
#include "log.h"

#include <QString>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <exception>
#include <regex>

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
    std::string msg = "Error opening file " + filename.toStdString();
    if (!file.is_open()) throw std::runtime_error(msg);
    std::vector<std::vector<std::string>> file_content;
    std::string line;
    while (std::getline(file, line))
    {
        file_content.push_back(split_csv_line(line));
    }

    file.close();
    log_info("File [" + filename.toStdString() + "] has been parsed.");
    return file_content;
}

std::vector<std::vector<std::string>> c_parse_file(const std::string& filename)
{
    std::ifstream file(filename);
//    if (file.bad()) throw std::runtime_error("File is considered bad.");
    std::string msg = "Error opening file " + filename;
    if (!file.is_open()) throw std::runtime_error(msg);

    std::vector<std::vector<std::string>> file_content;
    std::string line;
    while (std::getline(file, line))
    {
        file_content.push_back(split_csv_line(line));
    }

    file.close();
    log_info("File [" + filename + "] has been parsed.");
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

void write_csv(const std::vector<std::vector<std::string>>& table, const std::string& f_name)
{
    if (f_name.size() < 4 || f_name.substr(f_name.size() - 4) != ".csv")
    {
        throw std::runtime_error("File name must end with .csv");
    }

    std::ofstream file(f_name);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + f_name);
    }

    for (const auto& row : table)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            std::string val = row[i];
            bool needs_quotes = (val.find(',') != std::string::npos) || (val.find('"') != std::string::npos) || (val.find('\n') != std::string::npos);
            if (needs_quotes)
            {
                val = std::regex_replace(val, std::regex("\""), "\"\"");
                val = '"' + val + '"';
            }
            file << val;
            if (i < row.size() - 1)
            {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}
