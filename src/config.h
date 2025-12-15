#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    inline static bool CheckConditions = false;
    inline static bool AdditionalNameCheck = false;
    inline static const char* DefaultLogPath = "log.txt";
    inline static const char* DefaultOutPath = "out.csv";
    inline static std::string_view LogPath = DefaultLogPath;
    inline static std::string_view OutPath = DefaultOutPath;
    constexpr static bool DebugEnabled =
    #ifdef DEBUG_BUILD
        true;
    #else
        false;
    #endif
};

#endif // CONFIG_H
