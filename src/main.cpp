#include <iostream>
#include "../include/logger.h"
#include "../include/config.h"

int main()
{
    Logger::Log("Jarvis Started");

    Logger::Log("Configuration Loaded", LogLevel::DEBUG);

    Logger::Log("Microphone Connected", LogLevel::INFO);

    Logger::Log("Low Battery", LogLevel::WARNING);

    Logger::Log("Internet Connection Lost", LogLevel::ERROR);

    Config::Load();

    Logger::Log(
        "Assistant Name : " + Config::GetAssistantName());

    Logger::Log(
        "Language : " + Config::GetLanguage());

    Logger::Log(
        "Voice : " + Config::GetVoice());

    return 0;
}