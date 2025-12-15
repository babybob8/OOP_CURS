#include "book.h"
#include "log.h"
#include "config.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

void runTests()
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
        if (std::string(e.what()).find("Precondition violation") != std::string::npos)
        {
            log_info("Test 2: addStudent empty name violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 2 FAILED: Unexpected error - ") + e.what());
        }
    }

    try
    {
        GeneralStudPerfBook book;
        book.addWork("Math Exam", 100.0, true, true, true);
        log_info("Test 3: addWork and invariant - PASSED");
    }
    catch (const std::exception& e)
    {
        log_error(std::string("Test 3 FAILED: ") + e.what());
    }

    try
    {
        GeneralStudPerfBook book;
        book.addStudent("Alice");
        book.addWork("Test Work", 10.0, true, true, true);
        book.addResult("Alice", "Test Work", 15.0, 1, "", "");  // score > max
        log_error("Test 4: Should have thrown exception but didn't");
    }
    catch (const std::runtime_error& e)
    {
        if (std::string(e.what()).find("Precondition violation: Score exceeds max") != std::string::npos)
        {
            log_info("Test 4: addResult score violation - PASSED");
        }
        else
        {
            log_error(std::string("Test 4 FAILED: Unexpected error - ") + e.what());
        }
    }

    log_info("All tests completed.");
}
