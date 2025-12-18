#include "logger.h"
#include "config.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::function<void(const std::string& errorMsg)> logger::s_errorHandler = nullptr;

logger& logger::instance()
{
    static logger inst(Config::LogPath);
    return inst;
}

void logger::setErrorHandler(std::function<void(const std::string& errorMsg)> handler)
{
    s_errorHandler = std::move(handler);
}

void logger::resetErrorHandler()
{
    s_errorHandler = nullptr;
}

logger::logger(std::string_view f_name) : QObject(nullptr)
{
    m_file.open(f_name.data(), std::ofstream::out);

    if (!m_file.is_open())
    {
        std::string errorMsg = "Log file open failed: " + std::string(f_name);
        if (s_errorHandler)
        {
            s_errorHandler(errorMsg);
        }
        else
        {
            throw std::runtime_error(errorMsg);
        }
    }
}

logger::~logger()
{
    if (m_file.is_open())
        m_file.close();
}

void logger::log(const std::string& msg, logLevel level) noexcept
{
    if (!m_file.is_open())
        return;

    std::string timestamp = getCurrentTime();
    std::string levelStr = logLevelToString(level);
    std::string formatted = "[ " + timestamp + " ] [ " + levelStr + " ] " + msg;

    m_file << formatted << '\n';
    m_file.flush();

    emit newLog(QString::fromStdString(formatted));
}

std::string logger::logLevelToString(logLevel level) const noexcept
{
    switch (level)
    {
        case logLevel::debug:    return "DEBUG";
        case logLevel::info:     return "INFO";
        case logLevel::warn:     return "WARNING";
        case logLevel::error:    return "ERROR";
        case logLevel::critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string logger::getCurrentTime() const noexcept
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &time_t_now);
#else
    localtime_r(&time_t_now, &tm);
#endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
