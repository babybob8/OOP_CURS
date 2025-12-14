#ifndef LOG_H
#define LOG_H

#include "logger.h"
#include "config.h"

inline void log_debug(const std::string& msg)
{
    if constexpr (Config::DebugEnabled) logger::instance().log(msg, logLevel::debug);
}

inline void log_info(const std::string& msg)
{
    logger::instance().log(msg, logLevel::info);
}

inline void log_warning(const std::string& msg)
{
    logger::instance().log(msg, logLevel::warn);
}

inline void log_error(const std::string& msg)
{
    logger::instance().log(msg, logLevel::error);
}

inline void log_critical(const std::string& msg)
{
    logger::instance().log(msg, logLevel::critical);
}

#endif // LOG_H
