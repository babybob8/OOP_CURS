#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config
{
    constexpr std::string_view DefaultLogPath = "log.txt";
    constexpr std::string_view DefaultOutPath = "out.csv";

    constexpr bool DebugEnabled =
    #ifdef DEBUG_BUILD
        true;
    #else
        false;
    #endif

    extern bool CheckConditions;
    extern bool AdditionalNameCheck;
    inline std::string_view LogPath = DefaultLogPath;
    inline std::string_view OutPath = DefaultLogPath;
}

#endif // CONFIG_H
