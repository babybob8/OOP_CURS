#include "book.h"
#include "parse.h"
#include "logger.h"
#include "log.h"
#include "config.h"
#include "tests.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <exception>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif

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
    log_info("Application started.");
    log_debug("In Debug mode.");

    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    std::cout.imbue(std::locale(""));
    std::string students_csv;
    std::string works_csv;
    std::string results_csv;
    std::string out_path = std::string(Config::DefaultOutPath);
    bool show_help = false;
    bool run_tests = false;

    std::vector<std::string> positional_args;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--test")
        {
            run_tests = true;
        }
        else if (arg == "--help")
        {
            show_help = true;
            log_info("Showing help.");
        }
        else if (arg == "--out-path")
        {
            if (i + 1 < argc)
            {
                out_path = argv[++i];
                Config::OutPath = std::string_view(out_path);
                log_info("OutPath set to [" + out_path + "] via special key.");
            }
            else
            {
                std::cerr << "Error: --out-path requires an argument." << std::endl;
                log_error("Missing argument for --out-path.");
                return 1;
            }
        }
        else if (arg == "-c")
        {
            Config::CheckConditions = true;
            log_info("Condition checks enabled via special key.");

        }
        else if (arg == "-a")
        {
            Config::AdditionalNameCheck = true;
            log_info("Additional name checks enabled via special key.");

        }
        else if (arg[0] == '-')
        {
            std::cerr << "Unknown option: " << arg << std::endl;
            log_error("Unknown option.");
            return 1;
        }
        else
        {
            positional_args.push_back(arg);
        }
    }

    if (run_tests)
    {
        runTests();
        return 0;
    }

    if (show_help)
    {
        std::cout << "Usage: " << argv[0] << " [options] <students.csv> <works.csv> <results.csv>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  --help        Show this help message" << std::endl;
        std::cout << "  --out-path <path>  Set output path (default: out.csv)" << std::endl;
        std::cout << "  -c            Enable check conditions" << std::endl;
        std::cout << "  -a            Enable additional name check" << std::endl;
        return 0;
    }

    if (positional_args.size() != 3)
    {
        std::cerr << "Usage: " << argv[0] << " [options] <students.csv> <works.csv> <results.csv>" << std::endl;
        log_error("Invalid arguments.");
        return 1;
    }

    students_csv = positional_args[0];
    works_csv = positional_args[1];
    results_csv = positional_args[2];
    try
    {
        auto studentsTable = c_parse_file(students_csv);
        auto worksTable = c_parse_file(works_csv);
        auto resultsTable = c_parse_file(results_csv);
        log_info("Files parsed successfully.");

        auto currentResults = generateCurrentResults(studentsTable, worksTable, resultsTable);
        log_info("Results generated.");

        try
        {
            write_csv(currentResults, Config::OutPath.data());
            log_info("Results written.");
        }
        catch (const std::exception& e)
        {
            log_error("Results were not written in +" + std::string(Config::OutPath.data())
                      + ". Error: " + std::string(e.what()));
        }

        std::cout << "Generated Results:" << std::endl;

        printTable(currentResults);
        log_info("Results printed.");
    }
    catch (const std::exception& e)
    {
        log_error(std::string("Exception: ") + e.what());
        return 1;
    }
    return 0;
}
