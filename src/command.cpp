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

    if (command == "help")
    {
        std::cout << "Available Commands\n";
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
        std::cout << "exit\n";
    }

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

    else if (command == "clear")
    {
        system("cls");
    }

    else if (command == "about")
    {
        std::cout << "Jarvis AI v1.0\n";
    }

    else if (parsed.action == "open")
    {
        AppLauncher::Open(parsed.target);
    }

    else if (parsed.action == "create")
    {
        if (parsed.target == "folder")
        {
            FileManager::CreateFolder(parsed.argument);
        }
        else
        {
            std::cout << "Unknown create command\n";
        }
    }

    else if (parsed.action == "delete")
    {
        if (parsed.target == "folder")
        {
            FileManager::DeleteFolder(parsed.argument);
        }
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
        }
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
        }
        else if (parsed.action == "rename")
        {
            FileManager::Rename(parsed.argument,
                                parsed.argument2);
        }
        else if (parsed.action == "list")
        {
            if (parsed.target == "files")
            {
                FileManager::ListFiles();
            }
        }
    }
    else if (parsed.action == "system" &&
             parsed.target == "info")
    {
        SystemManager::SystemInfo();
    }

    else if (parsed.action == "google")
    {
        WebManager::GoogleSearch(parsed.target);
    }

    else if (parsed.action == "youtube")
    {
        WebManager::YouTubeSearch(parsed.target);
    }

    else if (parsed.action == "github")
    {
        WebManager::GitHubSearch(parsed.target);
    }

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
    else if (command == "exit")
    {
        std::cout << "Goodbye!\n";
        return false;
    }

    else
    {
        std::cout << "Unknown Command\n";
    }

    return true;
}