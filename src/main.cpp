#include <iostream>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/command.h"
#include "../include/speech_manager.h"
#include "../include/ai_manager.h"

int main()
{
    // -----------------------------
    // Startup
    // -----------------------------

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

    // -----------------------------
    // Speech Initialization
    // -----------------------------

    if (!SpeechManager::Initialize())
    {
        Logger::Log(
            "Speech initialization failed",
            LogLevel::ERROR);

        return 1;
    }

    // -----------------------------
    // AI Initialization
    // -----------------------------

    AIManager::Initialize();

    std::cout
        << "\n=================================\n"
        << "          JARVIS READY\n"
        << "=================================\n"
        << "Speak naturally.\n"
        << "Say goodbye to exit.\n\n";

    // -----------------------------
    // Main Loop
    // -----------------------------

    bool running = true;

    while (running)
    {
        std::string cmd = SpeechManager::Listen();

        // Ignore empty / blank audio
        if (cmd.empty() || cmd == "[BLANK_AUDIO]")
        {
            continue;
        }

        std::cout
            << "\nRecognized Text: "
            << cmd
            << "\n";

        // Send recognized speech to command processor
        running = Command::ProcessCommand(cmd);
    }

    // -----------------------------
    // Shutdown
    // -----------------------------

    SpeechManager::Shutdown();

    Logger::Log(
        "Jarvis Stopped",
        LogLevel::INFO);

    return 0;
}