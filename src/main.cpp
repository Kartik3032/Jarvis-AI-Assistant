#include <iostream>
#include <string>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/command.h"
#include "../include/speech_manager.h"
#include "../include/ai_manager.h"

int main()
{
    Logger::Log(
        "Jarvis Started",
        LogLevel::INFO);

    Config::Load();

    Logger::Log(
        "Configuration Loaded",
        LogLevel::DEBUG);

    Logger::Log(
        "Assistant Name : " +
        Config::GetAssistantName());

    Logger::Log(
        "Language : " +
        Config::GetLanguage());

    Logger::Log(
        "Voice : " +
        Config::GetVoice());

    if (!SpeechManager::Initialize())
    {
        Logger::Log(
            "Speech initialization failed",
            LogLevel::ERROR);

        return 1;
    }

    AIManager::Initialize();

    std::cout
        << "\n=================================\n"
        << "          JARVIS READY\n"
        << "=================================\n"
        << "Speak naturally.\n"
        << "Say goodbye to exit.\n\n";

    bool running = true;

    while (running)
    {
        std::string command =
            SpeechManager::Listen();

        if (command.empty() ||
            command == "[BLANK_AUDIO]")
        {
            continue;
        }

        std::cout
            << "\nRecognized Text: "
            << command
            << "\n";

        running =
            Command::ProcessCommand(command);
    }

    Logger::Log(
        "Jarvis Stopped",
        LogLevel::INFO);

    return 0;
}