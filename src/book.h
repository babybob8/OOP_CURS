#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

struct Work
{
    std::string name;
    double max_score;
    bool add_scores;
    bool mandatory;
    bool include_in_report;
};

class IGeneralStudPerfBook
{
public:
    virtual ~IGeneralStudPerfBook() = default;

    virtual void addStudent(const std::string& name) = 0;
    virtual void addWork(const std::string& name, double max_score, bool add_scores, bool mandatory, bool include_in_report) = 0;
    virtual void addResult(const std::string& student, const std::string& work, double score,
                           int attempt, std::string date, std::string notes) = 0;

    virtual std::vector<std::string> getAllStudents() const noexcept = 0;
    virtual std::vector<std::string> getQualifiedStudents() const noexcept = 0;
    virtual double getStudTotalScore(const std::string& name) const = 0;
    virtual std::vector<std::vector<std::string>> getCurrentResults() const = 0;
};

class GeneralStudPerfBook : public IGeneralStudPerfBook
{
private:
    std::map<std::string, std::map<std::string, double>> m_scores;
    std::vector<std::string> m_work_order;
    std::map<std::string, Work> m_works;

    bool invariantCheck() const noexcept;
    bool postconditionCheck() const noexcept;
    void validateName(const std::string& name) const;

public:
    GeneralStudPerfBook() = default;

    void addStudent(const std::string& name) override;
    void addWork(const std::string& name, double max_score, bool add_scores, bool mandatory, bool include_in_report) override;
    void addResult(const std::string& student, const std::string& work, double score,
                   int attempt, std::string date, std::string notes) override;

    std::vector<std::string> getAllStudents() const noexcept override;
    std::vector<std::string> getQualifiedStudents() const noexcept override;
    double getStudTotalScore(const std::string& name) const override;
    std::vector<std::vector<std::string>> getCurrentResults() const override;
};

class GeneralStudPerfBookDecorator : public IGeneralStudPerfBook
{
protected:
    std::shared_ptr<IGeneralStudPerfBook> m_wrapped;

public:
    GeneralStudPerfBookDecorator(std::shared_ptr<IGeneralStudPerfBook> component) : m_wrapped(component) {}

    void addStudent(const std::string& name) override { m_wrapped->addStudent(name); }
    void addWork(const std::string& name, double max_score, bool add_scores, bool mandatory, bool include_in_report) override
    { m_wrapped->addWork(name, max_score, add_scores, mandatory, include_in_report); }
    void addResult(const std::string& student, const std::string& work, double score,
                   int attempt, std::string date, std::string notes) override
    { m_wrapped->addResult(student, work, score, attempt, date, notes); }

    std::vector<std::string> getAllStudents() const noexcept override { return m_wrapped->getAllStudents(); }
    std::vector<std::string> getQualifiedStudents() const noexcept override { return m_wrapped->getQualifiedStudents(); }
    double getStudTotalScore(const std::string& name) const override { return m_wrapped->getStudTotalScore(name); }
    std::vector<std::vector<std::string>> getCurrentResults() const override { return m_wrapped->getCurrentResults(); }
};

class NameValidationDecorator : public GeneralStudPerfBookDecorator
{
public:
    NameValidationDecorator(std::shared_ptr<IGeneralStudPerfBook> component) : GeneralStudPerfBookDecorator(component) {}

    void addStudent(const std::string& name) override;
    void addResult(const std::string& student, const std::string& work, double score,
                   int attempt, std::string date, std::string notes) override;
};

std::string trim(std::string s);

void loadTablesIntoBook(IGeneralStudPerfBook& book,
                        const std::vector<std::vector<std::string>>& students_table,
                        const std::vector<std::vector<std::string>>& works_table,
                        const std::vector<std::vector<std::string>>& results_table);

std::vector<std::vector<std::string>> generateCurrentResults(
    const std::vector<std::vector<std::string>>& students_table,
    const std::vector<std::vector<std::string>>& works_table,
    const std::vector<std::vector<std::string>>& results_table);

#endif // BOOK_H
