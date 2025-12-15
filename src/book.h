#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>

struct Work
{
    std::string name;
    double max_score;
    bool add_scores;
    bool mandatory;
    bool include_in_report;
};

class GeneralStudPerfBook
{
private:
    std::map<std::string, std::map<std::string, double>> m_scores;
    std::vector<std::string> m_work_order;
    std::map<std::string, Work> m_works;

    bool invariantCheck() const noexcept;
    void validateName(const std::string& name) const;

public:
    GeneralStudPerfBook() = default;

    void addStudent(const std::string& name);
    void addWork(const std::string& name, double max_score, bool add_scores, bool mandatory, bool include_in_report);
    void addResult(const std::string& student, const std::string& work, double score,
                   int attempt, std::string date, std::string notes);

    std::vector<std::string> getAllStudents() const noexcept;
    std::vector<std::string> getQualifiedStudents() const noexcept;
    double getStudTotalScore(const std::string& name) const;

    std::vector<std::vector<std::string>> getCurrentResults() const;
};

std::string trim(std::string s);

void loadTablesIntoBook(GeneralStudPerfBook& book,
                        const std::vector<std::vector<std::string>>& students_table,
                        const std::vector<std::vector<std::string>>& works_table,
                        const std::vector<std::vector<std::string>>& results_table);

std::vector<std::vector<std::string>> generateCurrentResults(
    const std::vector<std::vector<std::string>>& students_table,
    const std::vector<std::vector<std::string>>& works_table,
    const std::vector<std::vector<std::string>>& results_table);

#endif // BOOK_H
