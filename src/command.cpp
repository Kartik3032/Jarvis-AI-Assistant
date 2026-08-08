#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

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

std::string Command::GetCommand()
{
    std::cout << "\nJarvis > ";

    std::string command;

    std::getline(std::cin, command);

    return command;
}

bool Command::ProcessCommand(const std::string &command)
{
    ParsedCommand parsed = Parser::Parse(command);

    // HELP
    if (command == "help")
    {
        std::cout << "\nAvailable Commands\n";
        std::cout << "help\n";
        std::cout << "time\n";
        std::cout << "date\n";
        std::cout << "clear\n";
        std::cout << "about\n";

        std::cout << "open chrome\n";
        std::cout << "open calculator\n";
        std::cout << "open notepad\n";
        std::cout << "open youtube\n";

        std::cout << "create folder <name>\n";
        std::cout << "delete folder <name>\n";
        std::cout << "delete file <name>\n";
        std::cout << "rename <old> <new>\n";
        std::cout << "list files\n";

        std::cout << "system info\n";

        std::cout << "google <query>\n";
        std::cout << "youtube <query>\n";
        std::cout << "github <query>\n";

        std::cout << "chatgpt\n";
        std::cout << "gmail\n";
        std::cout << "google\n";
        std::cout << "youtube\n";
        std::cout << "github\n";

        std::cout << "calculate <number> <operator> <number>\n";

        std::cout << "note <text>\n";
        std::cout << "notes\n";

        std::cout << "remind <text>\n";
        std::cout << "reminders\n";

        std::cout << "exit\n";
    }

    // TIME
    else if (command == "time")
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};

#ifdef _WIN32
        localtime_s(&localTime, &t);
#else
        localtime_r(&t, &localTime);
#endif

        std::cout << "Current Time : "
                  << std::put_time(&localTime, "%H:%M:%S")
                  << std::endl;
    }

    // DATE
    else if (command == "date")
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};

#ifdef _WIN32
        localtime_s(&localTime, &t);
#else
        localtime_r(&t, &localTime);
#endif

        std::cout << "Current Date : "
                  << std::put_time(&localTime, "%d-%m-%Y")
                  << std::endl;
    }

    // CLEAR
    else if (command == "clear")
    {
        system("cls");
    }

    // ABOUT
    else if (command == "about")
    {
        std::cout << "Jarvis AI v1.0\n";
    }

    // APP LAUNCHER
    else if (parsed.action == "open")
    {
        AppLauncher::Open(parsed.target);
    }

    // CREATE
    else if (parsed.action == "create")
    {
        if (parsed.target == "folder")
        {
            FileManager::CreateFolder(parsed.argument);
        }
        else if (parsed.target == "file")
        {
            FileManager::CreateFile(parsed.argument);
        }
        else
        {
            std::cout << "Unknown create command\n";
        }
    }

    // DELETE
    else if (parsed.action == "delete")
    {
        if (parsed.target == "folder")
        {
            FileManager::DeleteFolder(parsed.argument);
        }
        else if (parsed.target == "file")
        {
            FileManager::DeleteFile(parsed.argument);
        }
        else
        {
            std::cout << "Unknown delete command\n";
        }
    }

    // RENAME
    else if (parsed.action == "rename")
    {
        FileManager::Rename(
            parsed.argument,
            parsed.argument2);
    }

    // LIST
    else if (parsed.action == "list")
    {
        if (parsed.target == "files")
        {
            FileManager::ListFiles();
        }
        else
        {
            std::cout << "Unknown list command\n";
        }
    }

    // SYSTEM INFO
    else if (parsed.action == "system" &&
             parsed.target == "info")
    {
        SystemManager::SystemInfo();
    }

    // GOOGLE SEARCH
    else if (parsed.action == "google")
    {
        WebManager::GoogleSearch(parsed.target);
    }

    // YOUTUBE SEARCH
    else if (parsed.action == "youtube")
    {
        WebManager::YouTubeSearch(parsed.target);
    }

    // GITHUB SEARCH
    else if (parsed.action == "github")
    {
        WebManager::GitHubSearch(parsed.target);
    }

    // DIRECT WEBSITES
    else if (command == "chatgpt")
    {
        WebManager::OpenChatGPT();
    }

    else if (command == "gmail")
    {
        WebManager::OpenGmail();
    }

    else if (command == "google")
    {
        WebManager::OpenGoogle();
    }

    else if (command == "youtube")
    {
        WebManager::OpenYouTube();
    }

    else if (command == "github")
    {
        WebManager::OpenGitHub();
    }

    // CALCULATOR
    else if (parsed.action == "calculate")
    {
        Calculator::Calculate(
            parsed.target + " " +
            parsed.argument + " " +
            parsed.argument2);
    }

    // NOTES
    else if (parsed.action == "note")
    {
        NotesManager::AddNote(
            parsed.target + " " +
            parsed.argument);
    }

    else if (command == "notes")
    {
        NotesManager::ShowNotes();
    }

    // REMINDERS
    else if (parsed.action == "remind")
    {
        ReminderManager::AddReminder(
            parsed.target + " " +
            parsed.argument + " " +
            parsed.argument2);
    }

    else if (command == "reminders")
    {
        ReminderManager::ShowReminders();
    }
    else if (parsed.action == "weather")
    {
        WeatherManager::GetWeather(parsed.target);
    }
    else if (command == "news")
    {
        NewsManager::GetNews();
    }
    else if (command == "battery")
    {
        BatteryManager::ShowBattery();
    }

    else if (command == "exit")
    {
        std::cout << "Goodbye!\n";
        return false;
    }

    // UNKNOWN
    else
    {
        std::cout << "Unknown Command\n";
    }

    return true;
}