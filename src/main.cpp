#include <iostream>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/command.h"
#include "../include/speech_manager.h"
#include "../include/ai_manager.h"
#include "../include/api_server.h"

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

    // -----------------------------
    // AI
    // -----------------------------

    AIManager::Initialize();

    // -----------------------------
    // API SERVER
    // -----------------------------

    APIServer::Start(8080);

    std::cout
        << "\n=================================\n"
        << "          JARVIS READY\n"
        << "=================================\n"
        << "API: http://localhost:8080\n"
        << "UI can now control JARVIS.\n"
        << "Say goodbye to exit.\n\n";

    // -----------------------------
    // Speech
    // -----------------------------

    if (!SpeechManager::Initialize())
    {
        Logger::Log(
            "Speech initialization failed",
            LogLevel::ERROR);
    }

    bool running = true;

    while (running)
    {
        std::string cmd =
            SpeechManager::Listen();

        if (cmd.empty() ||
            cmd == "[BLANK_AUDIO]")
        {
            continue;
        }

        std::cout
            << "\nRecognized Text: "
            << cmd
            << "\n";

        running =
            Command::ProcessCommand(cmd);
    }

    // -----------------------------
    // Shutdown
    // -----------------------------

    SpeechManager::Shutdown();

    APIServer::Stop();

    Logger::Log(
        "Jarvis Stopped",
        LogLevel::INFO);

    return 0;
}