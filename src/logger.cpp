#include "../include/logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

void Logger::Log(const std::string &message, LogLevel level)
{
    std::cout
        << "["
        << GetTimeStamp()
        << "] "
        << "["
        << LevelToString(level)
        << "] "
        << message
        << std::endl;
}

std::string Logger::GetTimeStamp()
{
    auto now = std::chrono::system_clock::now();

    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::stringstream ss;

    ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

std::string Logger::LevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::INFO:
        return "INFO";

    case LogLevel::WARNING:
        return "WARNING";

    case LogLevel::ERROR:
        return "ERROR";

    case LogLevel::DEBUG:
        return "DEBUG";

    default:
        return "UNKNOWN";
    }
}