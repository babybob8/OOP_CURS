#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <functional>

#include <QObject>

enum class logLevel
{
    debug,
    info,
    warn,
    error,
    critical
};

class logger : public QObject
{
    Q_OBJECT

public:
    static logger& instance();
    static void setErrorHandler(std::function<void(const std::string& errorMsg)> handler);
    static void resetErrorHandler();

    void log(const std::string& msg, logLevel level) noexcept;

signals:
    void newLog(const QString& formattedMessage);

private:
    logger(std::string_view f_name);
    ~logger();

    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    std::string logLevelToString(logLevel level) const noexcept;
    std::string getCurrentTime() const noexcept;

    std::ofstream m_file;
    static std::function<void(const std::string& errorMsg)> s_errorHandler;
};

#endif // LOGGER_H
