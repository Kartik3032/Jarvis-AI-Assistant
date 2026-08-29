#include <iostream>
#include <string>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/command.h"
#include "../include/speech_manager.h"
#include "../include/ai_manager.h"
#include "../include/api_server.h"

int main()
{
    // ==========================================
    // START JARVIS
    // ==========================================

    Logger::Log(
        "Jarvis Started",
        LogLevel::INFO);

    // ==========================================
    // CONFIGURATION
    // ==========================================

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

    // ==========================================
    // SPEECH
    // ==========================================

    if (!SpeechManager::Initialize())
    {
        Logger::Log(
            "Speech initialization failed",
            LogLevel::ERROR);

        return 1;
    }

    // ==========================================
    // AI
    // ==========================================

    AIManager::Initialize();

    // ==========================================
    // API SERVER
    // ==========================================

    if (!APIServer::Start(8080))
    {
        Logger::Log(
            "API server failed to start",
            LogLevel::ERROR);

        SpeechManager::Shutdown();

        return 1;
    }

    // ==========================================
    // JARVIS READY
    // ==========================================

    std::cout
        << "\n=================================\n"
        << "          JARVIS READY\n"
        << "=================================\n"
        << "API Server : http://localhost:8080\n"
        << "Frontend   : http://localhost:5173\n"
        << "Voice      : Active\n"
        << "Mode       : Manual Listening\n"
        << "Say goodbye to exit.\n\n";

    bool running = true;

    // ==========================================
    // MAIN VOICE LOOP
    // ==========================================

    while (running)
    {
        std::string text =
            SpeechManager::Listen();

        if (text.empty() ||
            text == "[BLANK_AUDIO]")
        {
            continue;
        }

        std::cout
            << "\nRecognized Text: "
            << text
            << "\n";

        running =
            Command::ProcessCommand(text);
    }

    // ==========================================
    // SHUTDOWN
    // ==========================================

    APIServer::Stop();

    SpeechManager::Shutdown();

    Logger::Log(
        "Jarvis Stopped",
        LogLevel::INFO);

    return 0;
}