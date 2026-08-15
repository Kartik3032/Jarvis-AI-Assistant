#include "../include/command.h"

#include "../include/parser.h"
#include "../include/app_launcher.h"
#include "../include/file_manager.h"
#include "../include/system_manager.h"
#include "../include/web_manager.h"
#include "../include/calculator.h"
#include "../include/notes_manager.h"
#include "../include/reminder_manager.h"
#include "../include/weather_manager.h"
#include "../include/news_manager.h"
#include "../include/battery_manager.h"
#include "../include/volume_manager.h"
#include "../include/music_manager.h"
#include "../include/tts_manager.h"
#include "../include/ai_manager.h"

#include <windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#pragma comment(lib, "Shell32.lib")

namespace
{
    std::string Lower(
        std::string text)
    {
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(
                    std::tolower(c));
            });

        return text;
    }

    void Speak(
        const std::string &text)
    {
        std::cout
            << "\nJarvis: "
            << text
            << "\n";

        TTSManager::Speak(text);
    }

    void OpenURL(
        const char *url)
    {
        ShellExecuteA(
            nullptr,
            "open",
            url,
            nullptr,
            nullptr,
            SW_SHOWNORMAL);
    }

    std::string After(
        const std::string &text,
        const std::string &prefix)
    {
        if (text.size() <= prefix.size())
            return "";

        return text.substr(prefix.size());
    }
}

bool Command::ProcessCommand(
    const std::string &command)
{
    if (command.empty())
        return true;

    std::string cmd =
        Lower(command);

    std::cout
        << "\n=================================\n"
        << "         COMMAND PROCESSOR\n"
        << "=================================\n"
        << "Command: "
        << command
        << "\n";

    // =================================
    // EXIT
    // =================================

    if (cmd == "exit" ||
        cmd == "quit" ||
        cmd == "goodbye" ||
        cmd.find("stop assistant") !=
            std::string::npos)
    {
        Speak(
            "Goodbye. Shutting down.");

        return false;
    }

    // =================================
    // HELP
    // =================================

    if (cmd == "help" ||
        cmd == "what can you do")
    {
        Speak(
            "I can open applications, "
            "search the web, calculate, "
            "check weather, manage files, "
            "play music, take notes, "
            "set reminders, and have conversations.");

        return true;
    }

    // =================================
    // HELLO
    // =================================

    if (cmd == "hello" ||
        cmd == "hi" ||
        cmd == "hey" ||
        cmd.find("hello jarvis") !=
            std::string::npos ||
        cmd.find("hey jarvis") !=
            std::string::npos)
    {
        Speak(
            "Hello! How can I help you?");

        return true;
    }

    // =================================
    // TIME
    // =================================

    if (cmd == "time" ||
        cmd.find("what time") !=
            std::string::npos)
    {
        SYSTEMTIME time;

        GetLocalTime(&time);

        char buffer[100];

        sprintf_s(
            buffer,
            "%02d:%02d",
            time.wHour,
            time.wMinute);

        std::string response =
            "The current time is " +
            std::string(buffer);

        Speak(response);

        return true;
    }

    // =================================
    // OPEN CHROME
    // =================================

    if (cmd.find("open chrome") !=
        std::string::npos)
    {
        Speak(
            "Opening Chrome.");

        OpenURL(
            "https://www.google.com");

        return true;
    }

    // =================================
    // YOUTUBE
    // =================================

    if (cmd == "youtube" ||
        cmd.find("open youtube") !=
            std::string::npos)
    {
        Speak(
            "Opening YouTube.");

        WebManager::OpenYouTube();

        return true;
    }

    // =================================
    // YOUTUBE SEARCH
    // =================================

    if (cmd.find("search youtube") == 0)
    {
        std::string query =
            After(cmd, "search youtube");

        if (query.empty())
        {
            Speak(
                "What should I search for?");

            return true;
        }

        WebManager::YouTubeSearch(query);

        Speak(
            "Searching YouTube.");

        return true;
    }

    // =================================
    // GOOGLE SEARCH
    // =================================

    if (cmd.find("search google") == 0 ||
        cmd.find("google search") == 0)
    {
        std::string query;

        if (cmd.find("search google") == 0)
            query = After(cmd, "search google");
        else
            query = After(cmd, "google search");

        if (query.empty())
        {
            Speak(
                "What should I search for?");

            return true;
        }

        WebManager::GoogleSearch(query);

        Speak(
            "Searching Google.");

        return true;
    }

    // =================================
    // GOOGLE
    // =================================

    if (cmd == "google" ||
        cmd == "open google")
    {
        WebManager::OpenGoogle();

        Speak(
            "Opening Google.");

        return true;
    }

    // =================================
    // GITHUB
    // =================================

    if (cmd == "github" ||
        cmd == "open github")
    {
        WebManager::OpenGitHub();

        Speak(
            "Opening GitHub.");

        return true;
    }

    // =================================
    // CHATGPT
    // =================================

    if (cmd == "chatgpt" ||
        cmd == "open chatgpt")
    {
        WebManager::OpenChatGPT();

        Speak(
            "Opening ChatGPT.");

        return true;
    }

    // =================================
    // CALCULATOR
    // =================================

    if (cmd.find("calculate ") == 0)
    {
        std::string expression =
            After(cmd, "calculate ");

        Calculator::Calculate(
            expression);

        Speak(
            "Calculation completed.");

        return true;
    }

    if (cmd == "open calculator" ||
        cmd == "calculator")
    {
        ShellExecuteA(
            nullptr,
            "open",
            "calc.exe",
            nullptr,
            nullptr,
            SW_SHOWNORMAL);

        Speak(
            "Opening calculator.");

        return true;
    }

    // =================================
    // WEATHER
    // =================================

    if (cmd.find("weather") !=
        std::string::npos)
    {
        std::string city = "Mumbai";

        size_t pos =
            cmd.find("weather");

        std::string remaining =
            cmd.substr(
                pos + 7);

        if (!remaining.empty())
        {
            if (remaining.find(" in ") == 0)
                remaining =
                    remaining.substr(4);

            else if (
                remaining.find(" ") == 0)
                )
                {
                    remaining =
                        remaining.substr(1);
                }

            if (!remaining.empty())
                city = remaining;
        }

        WeatherManager::GetWeather(
            city);

        Speak(
            "Here is the weather information.");

        return true;
    }

    // =================================
    // FILES
    // =================================

    if (cmd == "open files" ||
        cmd == "open file explorer" ||
        cmd == "file explorer")
    {
        ShellExecuteA(
            nullptr,
            "open",
            "explorer.exe",
            nullptr,
            nullptr,
            SW_SHOWNORMAL);

        Speak(
            "Opening file explorer.");

        return true;
    }

    // =================================
    // LIST FILES
    // =================================

    if (cmd == "list files")
    {
        FileManager::ListFiles();

        Speak(
            "Here are your files.");

        return true;
    }

    // =================================
    // DOCUMENTS
    // =================================

    if (cmd == "open documents")
    {
        ShellExecuteA(
            nullptr,
            "open",
            "shell:Personal",
            nullptr,
            nullptr,
            SW_SHOWNORMAL);

        Speak(
            "Opening documents.");

        return true;
    }

    // =================================
    // DOWNLOADS
    // =================================

    if (cmd == "open downloads")
    {
        ShellExecuteA(
            nullptr,
            "open",
            "shell:Downloads",
            nullptr,
            nullptr,
            SW_SHOWNORMAL);

        Speak(
            "Opening downloads.");

        return true;
    }

    // =================================
    // MUSIC
    // =================================

    if (cmd == "open music" ||
        cmd == "music")
    {
        ShellExecuteA(
            nullptr,
            "open",
            "shell:MyMusic",
            nullptr,
            nullptr,
            SW_SHOWNORMAL);

        Speak(
            "Opening your music.");

        return true;
    }

    if (cmd.find("play ") == 0)
    {
        std::string path =
            After(cmd, "play ");

        MusicManager::Play(path);

        Speak(
            "Playing music.");

        return true;
    }

    if (cmd == "pause music" ||
        cmd == "pause")
    {
        MusicManager::Pause();

        Speak(
            "Music paused.");

        return true;
    }

    if (cmd == "resume music" ||
        cmd == "resume")
    {
        MusicManager::Resume();

        Speak(
            "Music resumed.");

        return true;
    }

    if (cmd == "stop music")
    {
        MusicManager::Stop();

        Speak(
            "Music stopped.");

        return true;
    }

    // =================================
    // NOTES
    // =================================

    if (cmd.find("take note ") == 0)
    {
        std::string note =
            After(cmd, "take note ");

        NotesManager::AddNote(note);

        Speak(
            "Your note has been saved.");

        return true;
    }

    if (cmd.find("note ") == 0)
    {
        std::string note =
            After(cmd, "note ");

        NotesManager::AddNote(note);

        Speak(
            "Your note has been saved.");

        return true;
    }

    if (cmd == "show notes" ||
        cmd == "notes")
    {
        NotesManager::ShowNotes();

        Speak(
            "Here are your notes.");

        return true;
    }

    // =================================
    // REMINDERS
    // =================================

    if (cmd.find("remind me ") == 0)
    {
        std::string reminder =
            After(cmd, "remind me ");

        ReminderManager::AddReminder(
            reminder);

        Speak(
            "Your reminder has been saved.");

        return true;
    }

    if (cmd == "show reminders" ||
        cmd == "reminders")
    {
        ReminderManager::ShowReminders();

        Speak(
            "Here are your reminders.");

        return true;
    }

    // =================================
    // BATTERY
    // =================================

    if (cmd.find("battery") !=
        std::string::npos)
    {
        BatteryManager::ShowBattery();

        Speak(
            "Here is your battery status.");

        return true;
    }

    // =================================
    // VOLUME
    // =================================

    if (cmd == "volume up" ||
        cmd == "increase volume")
    {
        VolumeManager::VolumeUp();

        Speak(
            "Volume increased.");

        return true;
    }

    if (cmd == "volume down" ||
        cmd == "decrease volume")
    {
        VolumeManager::VolumeDown();

        Speak(
            "Volume decreased.");

        return true;
    }

    if (cmd == "mute")
    {
        VolumeManager::Mute();

        Speak(
            "Mute toggled.");

        return true;
    }

    // =================================
    // NEWS
    // =================================

    if (cmd == "news" ||
        cmd.find("latest news") !=
            std::string::npos)
    {
        NewsManager::GetNews();

        Speak(
            "Here are the latest news headlines.");

        return true;
    }

    // =================================
    // AI CONVERSATION
    // =================================

    std::cout
        << "[AI] Thinking...\n";

    std::string answer =
        AIManager::Chat(command);

    Speak(answer);

    return true;
}