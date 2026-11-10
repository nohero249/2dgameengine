#include "Logger.h"

std::vector<LogEntry> Logger::messages;

std::string GetDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", localTime);
    return buffer;
}

void Logger::Log(const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "[\033[32mLOG\033[0m] [" + GetDateTime() + "] - " + message;
    std::cout << logEntry.message << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "[\033[31mERR\033[0m] [" + GetDateTime() + "] - " + message;
    std::cout << logEntry.message << std::endl;

    messages.push_back(logEntry);
}

