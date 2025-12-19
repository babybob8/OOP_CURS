#include "book.h"
#include "log.h"
#include "config.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

inline void runTestss()
{
    log_info("Starting unit tests...");

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("John Doe");
        assert(book.getAllStudents().size() == 1);
        log_info("Test 1: addStudent normal - PASSED");
    }
    catch (const std::exception& e)
    {
        log_error(std::string("Test 1 FAILED: ") + e.what());
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("");
        log_error("Test 2: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be empty.") != std::string::npos)
        {
            log_info("Test 2: addStudent empty name violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 2 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("   ");
        log_error("Test 3: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be only spaces.") != std::string::npos)
        {
            log_info("Test 3: addStudent only spaces violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 3 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("John Doe");
        book.addStudent("John Doe");
        log_error("Test 4: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student already exists.") != std::string::npos)
        {
            log_info("Test 4: addStudent duplicate violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 4 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Math Exam", 100.0, true, true, true);
        log_info("Test 5: addWork normal - PASSED");
    }
    catch (const std::exception& e)
    {
        log_error(std::string("Test 5 FAILED: ") + e.what());
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("", 100.0, true, true, true);
        log_error("Test 6: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Work name cannot be empty.") != std::string::npos)
        {
            log_info("Test 6: addWork empty name violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 6 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("   ", 100.0, true, true, true);
        log_error("Test 7: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Work name cannot be only spaces.") != std::string::npos)
        {
            log_info("Test 7: addWork only spaces violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 7 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Math Exam", 100.0, true, true, true);
        book.addWork("Math Exam", 100.0, true, true, true);
        log_error("Test 8: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Work already exists.") != std::string::npos)
        {
            log_info("Test 8: addWork duplicate violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 8 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Math Exam", -10.0, true, true, true);
        log_error("Test 9: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Max score cannot be negative.") != std::string::npos)
        {
            log_info("Test 9: addWork negative max score violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 9 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("Alice", "Test Work", 15.0, 1, "", "");
        log_error("Test 10: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Score out of range.") != std::string::npos)
        {
            log_info("Test 10: addResult score > max violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 10 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("Alice", "Test Work", -5.0, 1, "", "");
        log_error("Test 11: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Score out of range.") != std::string::npos)
        {
            log_info("Test 11: addResult score < 0 violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 11 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addResult("Alice", "Nonexistent Work", 5.0, 1, "", "");
        log_error("Test 12: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: No such work found.") != std::string::npos)
        {
            log_info("Test 12: addResult no such work violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 12 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addResult("Alice", "", 5.0, 1, "", "");
        log_error("Test 13: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Work name cannot be empty or only spaces.") != std::string::npos)
        {
            log_info("Test 13: addResult work empty violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 13 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addResult("Alice", "   ", 5.0, 1, "", "");
        log_error("Test 14: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Work name cannot be empty or only spaces.") != std::string::npos)
        {
            log_info("Test 14: addResult work only spaces violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 14 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("", "Test Work", 5.0, 1, "", "");
        log_error("Test 15: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be empty.") != std::string::npos)
        {
            log_info("Test 15: addResult student empty violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 15 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("   ", "Test Work", 5.0, 1, "", "");
        log_error("Test 16: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be only spaces.") != std::string::npos)
        {
            log_info("Test 16: addResult student only spaces violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 16 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.getStudTotalScore("");
        log_error("Test 17: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be empty.") != std::string::npos)
        {
            log_info("Test 17: getStudTotalScore empty name violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 17 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.getStudTotalScore("   ");
        log_error("Test 18: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Student name cannot be only spaces.") != std::string::npos)
        {
            log_info("Test 18: getStudTotalScore only spaces violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 18 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.getStudTotalScore("Nonexistent");
        log_error("Test 19: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("No such student found.") != std::string::npos)
        {
            log_info("Test 19: getStudTotalScore no student violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 19 FAILED: ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("Alice", "Test Work", 5.0, 1, "", "");
        auto results = book.getCurrentResults();
        log_info("Test 20: getCurrentResults normal (no invariant violation) - PASSED");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Invariant violation") != std::string::npos)
        {
            log_info("Test 20: getCurrentResults invariant violation - TRIGGERED (but should not in correct impl)");
        }
        else
        {
            log_error(std::string("Test 20 FAILED: ") + e.what());
        }
    }

    log_info("All tests completed.");
}
