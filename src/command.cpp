#include <chrono>
#include <iomanip>
#include <ctime>

#include "../include/command.h"

#include <iostream>

std::string Command::GetCommand()
{
    std::cout << "\nJarvis > ";

    std::string command;

    std::getline(std::cin, command);

    return command;
}

bool Command::ProcessCommand(const std::string &command)
{
    if (command == "help")
    {
        std::cout << "Available Commands\n";
        std::cout << "help\n";
        std::cout << "time\n";
        std::cout << "date\n";
        std::cout << "clear\n";
        std::cout << "about\n";
        std::cout << "notepad\n";
        std::cout << "calculator\n";
        std::cout << "chrome\n";
        std::cout << "youtube\n";
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

    else if (command == "notepad")
    {
        system("start notepad");
    }

    else if (command == "calculator")
    {
        system("start calc");
    }

    else if (command == "chrome")
    {
        system("start chrome");
    }

    else if (command == "youtube")
    {
        system("start https://youtube.com");
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