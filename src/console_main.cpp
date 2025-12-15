#include "book.h"
#include "parse.h"
#include "logger.h"
#include "log.h"
#include "config.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <exception>
#include <algorithm>

#include <Windows.h>

size_t visual_length(const std::string& str)
{
    size_t len = 0;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if ((str[i] & 0xC0) != 0x80)
            ++len;
    }
    return len;
}

void printTable(const std::vector<std::vector<std::string>>& table)
{
    if (table.empty())
    {
        log_error("No data to display.");
        return;
    }

    std::vector<size_t> colWidths(table[0].size(), 0);
    for (const auto& row : table)
    {
        for (size_t i = 0; i < row.size(); ++i)
            colWidths[i] = std::max(colWidths[i], visual_length(row[i]));
    }

    for (size_t i = 0; i < table[0].size(); ++i)
    {
        std::cout << std::left << table[0][i];
        size_t padding = colWidths[i] + 2 - visual_length(table[0][i]);
        std::cout << std::string(padding, ' ');
    }
    std::cout << std::endl;

    for (size_t r = 1; r < table.size(); ++r)
    {
        for (size_t i = 0; i < table[r].size(); ++i)
        {
            std::cout << std::left << table[r][i];
            size_t padding = colWidths[i] + 2 - visual_length(table[r][i]);
            std::cout << std::string(padding, ' ');
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    log_info("Console app started.");

    SetConsoleOutputCP(65001);
    std::cout.imbue(std::locale(""));

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <students.csv> <works.csv> <results.csv>" << std::endl;
        log_error("Invalid arguments.");
        return 1;
    }

    try
    {
        std::string studentsFile = argv[1];
        std::string worksFile = argv[2];
        std::string resultsFile = argv[3];

        auto studentsTable = c_parse_file(studentsFile);
        auto worksTable = c_parse_file(worksFile);
        auto resultsTable = c_parse_file(resultsFile);

        log_info("Files parsed successfully.");

        auto currentResults = generateCurrentResults(studentsTable, worksTable, resultsTable);

        std::cout << "Generated Results:" << std::endl;
        printTable(currentResults);

        log_info("Results generated and printed.");
    }
    catch (const std::exception& e)
    {
        log_error(std::string("Exception: ") + e.what());
        return 1;
    }

    return 0;
}
