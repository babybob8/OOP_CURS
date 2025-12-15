#include "book.h"

#include <stdexcept>
#include <cctype>
#include <tuple>
#include <string_view>

#include <fstream>

std::string trim(std::string s)
{
    std::string_view v = s;

    v.remove_prefix(std::min(v.find_first_not_of(" \t\n\r"), v.size()));
    v.remove_suffix(v.size() - std::min(v.find_last_not_of(" \t\n\r") + 1, v.size()));

    return std::string(v);
}

bool GeneralStudPerfBook::invariantCheck() const noexcept
{
    for (const auto& [name, s] : m_scores)
    {
        for (const auto& [w, sc] : s)
        {
            if (sc < 0 || sc > m_works.at(w).max_score)
                return false;
        }
    }
    return true;
}

void GeneralStudPerfBook::validateName(const std::string& name) const
{
    if (name.empty())
        throw std::runtime_error("Precondition violation: Student name cannot be empty.");

//    for (char c : name)
//    {
//        if (!std::isalpha(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c)))
//            throw std::runtime_error("Precondition violation: Student name can only contain letters and spaces (no digits or special characters).");
//    }
}

void GeneralStudPerfBook::addStudent(const std::string& name)
{
    validateName(name);
    if (m_scores.count(name))
        throw std::runtime_error("Precondition violation: Student already exists.");

    auto trimmed_name =  trim(name);
    m_scores[trimmed_name] = {};
}

void GeneralStudPerfBook::addWork(const std::string& name, double max_score, bool add_scores, bool mandatory, bool include_in_report)
{
    if (m_works.count(name))
        throw std::runtime_error("Precondition violation: Work already exists.");

    m_works[name] = {name, max_score, add_scores, mandatory, include_in_report};
    m_work_order.push_back(name);
}

void GeneralStudPerfBook::addResult(const std::string& student, const std::string& work, double score,
               int attempt = 1, std::string date = "", std::string notes = "")
{
    validateName(student);
    if (m_works.find(work) == m_works.end())
        throw std::runtime_error("Precondition violation: No such work found.");

    if (score < 0 || score > m_works.at(work).max_score)
        throw std::runtime_error("Precondition violation: Score out of range.");

    if (m_scores.find(student) == m_scores.end())
        addStudent(student);

    auto& sc = m_scores[student][work];
    if (m_works.at(work).add_scores)
    {
        sc += score;
        if (sc > m_works.at(work).max_score)
            sc = m_works.at(work).max_score;
    }
    else
    {
        sc = std::max(sc, score);
    }
}

std::vector<std::string> GeneralStudPerfBook::getAllStudents() const noexcept
{
    std::vector<std::string> result;
    for (const auto& [name, s] : m_scores)
    {
        result.push_back(name);
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::string> GeneralStudPerfBook::getQualifiedStudents() const noexcept
{
    std::vector<std::string> result;
    for (const auto& [name, s] : m_scores)
    {
        bool qualified = true;
        for (const auto& [w, wk] : m_works)
        {
            if (wk.mandatory)
            {
                auto it = s.find(w);
                if (it == s.end() || it->second <= 0.0)
                {
                    qualified = false;
                    break;
                }
            }
        }
        if (qualified)
            result.push_back(name);

    }
    std::sort(result.begin(), result.end());
    return result;
}

double GeneralStudPerfBook::getStudTotalScore(const std::string& name) const
{
    auto it = m_scores.find(name);
    if (it == m_scores.end())
        throw std::runtime_error("No such student found.");

    double sum = 0.0;
    for (const auto& [w, sc] : it->second)
    {
        if (m_works.at(w).include_in_report)
            sum += sc;
    }

    return (sum > 100.0) ? 100.0 : sum;
}

std::vector<std::vector<std::string>> GeneralStudPerfBook::getCurrentResults() const
{
    if (!invariantCheck())
        throw std::runtime_error("Invariant violation: Scores out of range.");

    std::vector<std::string> inc_works;
    for (const auto& w : m_work_order)
    {
        if (m_works.at(w).include_in_report)
            inc_works.push_back(w);
    }
    std::vector<std::vector<std::string>> table;
    std::vector<std::string> header = {"student"};
    for (const auto& w : inc_works)
    {
        header.push_back(w);
    }

    table.push_back(header);
    auto students = getAllStudents();
    for (const auto& s : students)
    {
        std::vector<std::string> row = {s};
        for (const auto& w : inc_works)
        {
            auto it = m_scores.at(s).find(w);
            std::string score_str = (it != m_scores.at(s).end()) ? std::to_string(it->second) : "0";
            row.push_back(score_str);
        }
        table.push_back(row);
    }
    return table;
}

void loadTablesIntoBook(GeneralStudPerfBook& book,
                        const std::vector<std::vector<std::string>>& students_table,
                        const std::vector<std::vector<std::string>>& works_table,
                        const std::vector<std::vector<std::string>>& results_table)
{

    if (!students_table.empty())
    {
        auto header = students_table[0];
        size_t name_idx = std::string::npos;
        for (size_t i = 0; i < header.size(); ++i)
        {
            if (header[i] == "student" || header[i] == "nm" || header[i] == "name" || header[i] == "fname")
            {
                name_idx = i;
                break;
            }
        }
        if (name_idx == std::string::npos)
            throw std::runtime_error("No student name column found in students table.");

        for (size_t r = 1; r < students_table.size(); ++r)
        {
            std::string name = students_table[r][name_idx];
            book.addStudent(name);
        }
    }

    if (!works_table.empty())
    {
        auto header = works_table[0];
        size_t name_idx = std::string::npos, max_idx = std::string::npos, add_idx = std::string::npos,
               mand_idx = std::string::npos, inc_idx = std::string::npos;
        for (size_t i = 0; i < header.size(); ++i)
        {
            if (header[i] == "work") name_idx = i;
            else if (header[i] == "max_score") max_idx = i;
            else if (header[i] == "add_flag") add_idx = i;
            else if (header[i] == "mandatory") mand_idx = i;
            else if (header[i] == "include") inc_idx = i;
        }

        if (name_idx == std::string::npos || max_idx == std::string::npos || add_idx == std::string::npos ||
            mand_idx == std::string::npos || inc_idx == std::string::npos)
        {
            throw std::runtime_error("Missing columns in works table.");
        }

        for (size_t r = 1; r < works_table.size(); ++r)
        {
            std::string name = works_table[r][name_idx];
            double max_score = std::stod(works_table[r][max_idx]);
            bool add = std::stoi(works_table[r][add_idx]) != 0;
            bool mand = std::stoi(works_table[r][mand_idx]) != 0;
            bool inc = std::stoi(works_table[r][inc_idx]) != 0;
            book.addWork(name, max_score, add, mand, inc);
        }
    }

    if (!results_table.empty())
    {
        auto header = results_table[0];
        size_t stud_idx = std::string::npos, work_idx = std::string::npos, score_idx = std::string::npos,
               attempt_idx = std::string::npos, date_idx = std::string::npos, notes_idx = std::string::npos;
        for (size_t i = 0; i < header.size(); ++i)
        {
            if (header[i] == "student") stud_idx = i;
            else if (header[i] == "work") work_idx = i;
            else if (header[i] == "score") score_idx = i;
            else if (header[i] == "attempt") attempt_idx = i;
            else if (header[i] == "date") date_idx = i;
            else if (header[i] == "notes") notes_idx = i;
        }
        if (stud_idx == std::string::npos || work_idx == std::string::npos || score_idx == std::string::npos)
            throw std::runtime_error("Missing required columns in results table.");

        for (size_t r = 1; r < results_table.size(); ++r)
        {
            std::string student = results_table[r][stud_idx];
            std::string work = results_table[r][work_idx];
            double score = std::stod(results_table[r][score_idx]);
            int attempt = (attempt_idx != std::string::npos) ? std::stoi(results_table[r][attempt_idx]) : 1;
            std::string date = (date_idx != std::string::npos) ? results_table[r][date_idx] : "";
            std::string notes = (notes_idx != std::string::npos) ? results_table[r][notes_idx] : "";

            book.addResult(student, work, score, attempt, date, notes);
        }
    }
}

std::vector<std::vector<std::string>> generateCurrentResults(
    const std::vector<std::vector<std::string>>& students_table,
    const std::vector<std::vector<std::string>>& works_table,
    const std::vector<std::vector<std::string>>& results_table)
{
    GeneralStudPerfBook book;
    loadTablesIntoBook(book, students_table, works_table, results_table);
    return book.getCurrentResults();
}
