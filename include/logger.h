#pragma once

#include <string>

enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger
{
public:
    static void Log(const std::string &message,
                    LogLevel level = LogLevel::INFO);

private:
    static std::string GetTimeStamp();
    static std::string LevelToString(LogLevel level);
};
