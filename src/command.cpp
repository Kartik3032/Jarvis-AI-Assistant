#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <cstdlib>

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
#include "../include/speech_manager.h"

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

        std::cout << "weather <city>\n";
        std::cout << "news\n";
        std::cout << "battery\n";

        std::cout << "volume up\n";
        std::cout << "volume down\n";
        std::cout << "mute\n";

        std::cout << "play <file path>\n";
        std::cout << "pause\n";
        std::cout << "resume\n";
        std::cout << "stop\n";

        std::cout << "speak test\n";
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

        TTSManager::Speak("The current time is displayed.");
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

        TTSManager::Speak("Today's date is displayed.");
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

        TTSManager::Speak(
            "I am Jarvis, your personal AI assistant.");
    }

    // APP LAUNCHER
    else if (parsed.action == "open")
    {
        AppLauncher::Open(parsed.target);

        TTSManager::Speak(
            "Opening the application.");
    }

    // CREATE
    else if (parsed.action == "create")
    {
        if (parsed.target == "folder")
        {
            FileManager::CreateFolder(parsed.argument);

            TTSManager::Speak(
                "Folder created successfully.");
        }
        else if (parsed.target == "file")
        {
            FileManager::CreateFile(parsed.argument);

            TTSManager::Speak(
                "File created successfully.");
        }
        else
        {
            std::cout << "Unknown create command\n";
            TTSManager::Speak(
                "I don't understand that create command.");
        }
    }

    // DELETE
    else if (parsed.action == "delete")
    {
        if (parsed.target == "folder")
        {
            FileManager::DeleteFolder(parsed.argument);

            TTSManager::Speak(
                "Folder deleted successfully.");
        }
        else if (parsed.target == "file")
        {
            FileManager::DeleteFile(parsed.argument);

            TTSManager::Speak(
                "File deleted successfully.");
        }
        else
        {
            std::cout << "Unknown delete command\n";
            TTSManager::Speak(
                "I don't understand that delete command.");
        }
    }

    // RENAME
    else if (parsed.action == "rename")
    {
        FileManager::Rename(
            parsed.argument,
            parsed.argument2);

        TTSManager::Speak(
            "Rename operation completed.");
    }

    // LIST
    else if (parsed.action == "list")
    {
        if (parsed.target == "files")
        {
            FileManager::ListFiles();

            TTSManager::Speak(
                "Here are the files.");
        }
        else
        {
            std::cout << "Unknown list command\n";
        }
    }

    // SYSTEM INFO
    else if (
        parsed.action == "system" &&
        parsed.target == "info")
    {
        SystemManager::SystemInfo();

        TTSManager::Speak(
            "System information is displayed.");
    }

    // GOOGLE SEARCH
    else if (parsed.action == "google")
    {
        WebManager::GoogleSearch(parsed.target);

        TTSManager::Speak(
            "Searching Google.");
    }

    // YOUTUBE SEARCH
    else if (parsed.action == "youtube")
    {
        WebManager::YouTubeSearch(parsed.target);

        TTSManager::Speak(
            "Searching YouTube.");
    }

    // GITHUB SEARCH
    else if (parsed.action == "github")
    {
        WebManager::GitHubSearch(parsed.target);

        TTSManager::Speak(
            "Searching GitHub.");
    }

    // DIRECT WEBSITES
    else if (command == "chatgpt")
    {
        WebManager::OpenChatGPT();

        TTSManager::Speak(
            "Opening ChatGPT.");
    }

    else if (command == "gmail")
    {
        WebManager::OpenGmail();

        TTSManager::Speak(
            "Opening Gmail.");
    }

    else if (command == "google")
    {
        WebManager::OpenGoogle();

        TTSManager::Speak(
            "Opening Google.");
    }

    else if (command == "youtube")
    {
        WebManager::OpenYouTube();

        TTSManager::Speak(
            "Opening YouTube.");
    }

    else if (command == "github")
    {
        WebManager::OpenGitHub();

        TTSManager::Speak(
            "Opening GitHub.");
    }

    // CALCULATOR
    else if (parsed.action == "calculate")
    {
        Calculator::Calculate(
            parsed.target + " " +
            parsed.argument + " " +
            parsed.argument2);

        TTSManager::Speak(
            "Calculation completed.");
    }

    // NOTES
    else if (parsed.action == "note")
    {
        NotesManager::AddNote(
            parsed.target + " " +
            parsed.argument);

        TTSManager::Speak(
            "Your note has been saved.");
    }

    else if (command == "notes")
    {
        NotesManager::ShowNotes();

        TTSManager::Speak(
            "Here are your saved notes.");
    }

    // REMINDERS
    else if (parsed.action == "remind")
    {
        ReminderManager::AddReminder(
            parsed.target + " " +
            parsed.argument + " " +
            parsed.argument2);

        TTSManager::Speak(
            "Your reminder has been saved.");
    }

    else if (command == "reminders")
    {
        ReminderManager::ShowReminders();

        TTSManager::Speak(
            "Here are your reminders.");
    }

    // WEATHER
    else if (parsed.action == "weather")
    {
        WeatherManager::GetWeather(parsed.target);

        TTSManager::Speak(
            "Here is the current weather information.");
    }

    // NEWS
    else if (command == "news")
    {
        NewsManager::GetNews();

        TTSManager::Speak(
            "Here are the latest news headlines.");
    }

    // BATTERY
    else if (command == "battery")
    {
        BatteryManager::ShowBattery();

        TTSManager::Speak(
            "Here is your current battery status.");
    }

    // VOLUME
    else if (command == "volume up")
    {
        VolumeManager::VolumeUp();

        TTSManager::Speak(
            "Volume increased.");
    }

    else if (command == "volume down")
    {
        VolumeManager::VolumeDown();

        TTSManager::Speak(
            "Volume decreased.");
    }

    else if (command == "mute")
    {
        VolumeManager::Mute();

        TTSManager::Speak(
            "Mute toggled.");
    }

    // MUSIC
    else if (parsed.action == "play")
    {
        MusicManager::Play(parsed.target);

        TTSManager::Speak(
            "Playing your music.");
    }

    else if (command == "pause")
    {
        MusicManager::Pause();

        TTSManager::Speak(
            "Music paused.");
    }

    else if (command == "resume")
    {
        MusicManager::Resume();

        TTSManager::Speak(
            "Music resumed.");
    }

    else if (command == "stop")
    {
        MusicManager::Stop();

        TTSManager::Speak(
            "Music stopped.");
    }

    // TTS TEST
    else if (command == "speak test")
    {
        TTSManager::Speak(
            "Hello, I am Jarvis. System is working.");
    }
    else if (command == "listen")
    {
        std::string spokenCommand = SpeechManager::Listen();

        if (!spokenCommand.empty())
        {
            ProcessCommand(spokenCommand);
        }
    }

    // EXIT
    else if (command == "exit")
    {
        TTSManager::Speak(
            "Goodbye. See you soon.");

        std::cout << "Goodbye!\n";

        return false;
    }

    // UNKNOWN
    else
    {
        std::cout << "Unknown Command\n";

        TTSManager::Speak(
            "Sorry, I don't understand that command.");
    }

    return true;
}