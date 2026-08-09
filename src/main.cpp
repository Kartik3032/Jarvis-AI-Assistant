#include <iostream>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/command.h"
#include "../include/parser.h"

#include <windows.h>
#undef ERROR

int main()
{

    Logger::Log("Jarvis Started");

    Logger::Log(
        "Configuration Loaded",
        LogLevel::DEBUG);

    Logger::Log(
        "Microphone Connected",
        LogLevel::INFO);

    Logger::Log(
        "Low Battery",
        LogLevel::WARNING);

    Logger::Log(
        "Internet Connection Lost",
        LogLevel::ERROR);

    Config::Load();

    Logger::Log(
        "Assistant Name : " +
        Config::GetAssistantName());

    Logger::Log(
        "Language : " +
        Config::GetLanguage());

    Logger::Log(
        "Voice : " +
        Config::GetVoice());

    Logger::Log(
        "Jarvis Started",
        LogLevel::INFO);

    bool running = true;

    while (running)
    {
        std::string cmd = Command::GetCommand();

        running = Command::ProcessCommand(cmd);
    }

    return 0;
}