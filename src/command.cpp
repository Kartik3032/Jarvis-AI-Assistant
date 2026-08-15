#include "../include/command.h"

#include "../include/calculator.h"
#include "../include/weather_manager.h"
#include "../include/web_manager.h"
#include "../include/music_manager.h"
#include "../include/file_manager.h"
#include "../include/notes_manager.h"
#include "../include/reminder_manager.h"
#include "../include/battery_manager.h"
#include "../include/volume_manager.h"
#include "../include/news_manager.h"
#include "../include/ai_manager.h"
#include "../include/tts_manager.h"

#include <algorithm>
#include <iostream>

namespace
{
    std::string Normalize(std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(std::tolower(c));
            });

        return text;
    }
}

bool Command::ProcessCommand(const std::string &input)
{
    if (input.empty())
        return true;

    std::string cmd = Normalize(input);

    std::cout << "\n=================================\n";
    std::cout << "        COMMAND PROCESSOR\n";
    std::cout << "=================================\n";
    std::cout << "Command: " << input << "\n";

    // -----------------------------
    // EXIT
    // -----------------------------

    if (cmd == "goodbye" ||
        cmd == "exit" ||
        cmd == "quit" ||
        cmd == "shutdown")
    {
        std::cout << "Jarvis: Goodbye!\n";
        TTSManager::Speak("Goodbye!");

        return false;
    }

    // -----------------------------
    // GREETING
    // -----------------------------

    if (cmd == "hello" ||
        cmd == "hi" ||
        cmd == "hey" ||
        cmd.find("hello jarvis") != std::string::npos)
    {
        std::string response =
            "Hello! How can I help you?";

        std::cout << "Jarvis: " << response << "\n";
        TTSManager::Speak(response);

        return true;
    }

    // -----------------------------
    // WEATHER
    // -----------------------------

    if (cmd.find("weather") != std::string::npos)
    {
        WeatherManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // CALCULATOR
    // -----------------------------

    if (cmd.find("calculate") != std::string::npos ||
        cmd.find("calculator") != std::string::npos ||
        cmd.find("what is") != std::string::npos)
    {
        Calculator::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // BROWSER / WEB
    // -----------------------------

    if (cmd.find("open google") != std::string::npos ||
        cmd.find("open youtube") != std::string::npos ||
        cmd.find("search") != std::string::npos ||
        cmd.find("browse") != std::string::npos)
    {
        WebManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // MUSIC
    // -----------------------------

    if (cmd.find("play music") != std::string::npos ||
        cmd.find("play song") != std::string::npos ||
        cmd.find("pause music") != std::string::npos ||
        cmd.find("stop music") != std::string::npos)
    {
        MusicManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // FILE MANAGER
    // -----------------------------

    if (cmd.find("open file") != std::string::npos ||
        cmd.find("find file") != std::string::npos ||
        cmd.find("list files") != std::string::npos)
    {
        FileManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // NOTES
    // -----------------------------

    if (cmd.find("take note") != std::string::npos ||
        cmd.find("note") != std::string::npos)
    {
        NotesManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // REMINDER
    // -----------------------------

    if (cmd.find("remind me") != std::string::npos ||
        cmd.find("reminder") != std::string::npos)
    {
        ReminderManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // BATTERY
    // -----------------------------

    if (cmd.find("battery") != std::string::npos)
    {
        BatteryManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // VOLUME
    // -----------------------------

    if (cmd.find("volume") != std::string::npos ||
        cmd.find("mute") != std::string::npos)
    {
        VolumeManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // NEWS
    // -----------------------------

    if (cmd.find("news") != std::string::npos)
    {
        NewsManager::HandleCommand(input);
        return true;
    }

    // -----------------------------
    // AI FALLBACK
    // -----------------------------

    std::cout << "Jarvis: Thinking...\n";

    std::string response =
        AIManager::Ask(input);

    if (!response.empty())
    {
        std::cout << "Jarvis: "
                  << response
                  << "\n";

        TTSManager::Speak(response);
    }
    else
    {
        std::cout
            << "Jarvis: I couldn't process that request.\n";
    }

    return true;
}