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
#include "../include/system_manager.h"
#include "../include/app_launcher.h"
#include "../include/ai_manager.h"
#include "../include/tts_manager.h"

#include <windows.h>
#include <shellapi.h>

// IMPORTANT:
// Windows defines CreateFile and DeleteFile as macros.
// They conflict with FileManager::CreateFile()
// and FileManager::DeleteFile().
#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#pragma comment(lib, "Shell32.lib")

namespace
{
    // ==========================================
    // LOWERCASE
    // ==========================================

    std::string Lower(std::string text)
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

    // ==========================================
    // TEXT TO SPEECH
    // ==========================================

    void Speak(const std::string &text)
    {
        std::cout
            << "Jarvis: "
            << text
            << "\n";

        TTSManager::Speak(text);
    }

    // ==========================================
    // GET TEXT AFTER PREFIX
    // ==========================================

    std::string After(
        const std::string &text,
        const std::string &prefix)
    {
        if (text.size() <= prefix.size())
            return "";

        return text.substr(prefix.size());
    }

    // ==========================================
    // OPEN WINDOWS FOLDER
    // ==========================================

    void OpenFolder(const char *folder)
    {
        ShellExecuteA(
            nullptr,
            "open",
            folder,
            nullptr,
            nullptr,
            SW_SHOWNORMAL);
    }

    // ==========================================
    // MAKE CALCULATOR EXPRESSION
    // ==========================================

    std::string MakeExpression(
        std::string text)
    {
        text = Lower(text);

        const std::string prefixes[] =
            {
                "calculate ",
                "what is ",
                "what's "};

        for (const auto &prefix : prefixes)
        {
            if (text.find(prefix) == 0)
            {
                text =
                    text.substr(prefix.size());

                break;
            }
        }

        // Natural language -> operators

        const std::string replacements[][2] =
            {
                {" divided by ", " / "},
                {" divide by ", " / "},
                {" multiplied by ", " * "},
                {" multiply by ", " * "},
                {" times ", " * "},
                {" plus ", " + "},
                {" minus ", " - "}};

        for (const auto &r : replacements)
        {
            size_t pos = 0;

            while (
                (pos = text.find(
                     r[0],
                     pos)) != std::string::npos)
            {
                text.replace(
                    pos,
                    r[0].length(),
                    r[1]);

                pos += r[1].length();
            }
        }

        return text;
    }
}

// ==========================================
// COMMAND PROCESSOR
// ==========================================

bool Command::ProcessCommand(
    const std::string &input)
{
    if (input.empty())
        return true;

    std::string cmd =
        Lower(input);

    std::cout
        << "\n=================================\n"
        << "         COMMAND PROCESSOR\n"
        << "=================================\n"
        << "Command: "
        << input
        << "\n";

    // ==========================================
    // EXIT
    // ==========================================

    if (cmd == "goodbye" ||
        cmd == "exit" ||
        cmd == "quit" ||
        cmd == "shutdown" ||
        cmd.find("stop jarvis") !=
            std::string::npos)
    {
        Speak(
            "Goodbye. See you later.");

        return false;
    }

    // ==========================================
    // GREETING
    // ==========================================

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

    // ==========================================
    // HELP
    // ==========================================

    if (cmd == "help" ||
        cmd.find("what can you do") !=
            std::string::npos)
    {
        Speak(
            "I can calculate, check weather, "
            "search the web, open websites, "
            "manage files, play music, "
            "take notes, create reminders, "
            "check battery, control volume, "
            "read news, and answer questions.");

        return true;
    }

    // ==========================================
    // TIME
    // ==========================================

    if (cmd.find("what time") !=
            std::string::npos ||
        cmd == "time" ||
        cmd.find("current time") !=
            std::string::npos)
    {
        SYSTEMTIME now;

        GetLocalTime(&now);

        char timeText[32];

        sprintf_s(
            timeText,
            "%02d:%02d",
            now.wHour,
            now.wMinute);

        Speak(
            "The current time is " +
            std::string(timeText));

        return true;
    }

    // ==========================================
    // CALCULATOR
    // ==========================================

    if (cmd.find("calculate") == 0 ||
        cmd.find("what is ") == 0 ||
        cmd.find("what's ") == 0)
    {
        std::string expression =
            MakeExpression(input);

        Calculator::Calculate(
            expression);

        Speak(
            "Calculation completed.");

        return true;
    }

    // ==========================================
    // WEATHER
    // ==========================================

    if (cmd.find("weather") !=
        std::string::npos)
    {
        std::string city =
            "Mumbai";

        size_t weatherPos =
            cmd.find("weather");

        std::string remaining =
            cmd.substr(
                weatherPos + 7);

        while (
            !remaining.empty() &&
            remaining[0] == ' ')
        {
            remaining.erase(0, 1);
        }

        if (remaining.find("in ") == 0)
        {
            remaining =
                remaining.substr(3);
        }

        if (!remaining.empty())
        {
            city = remaining;
        }

        std::cout
            << "Checking weather for: "
            << city
            << "\n";

        WeatherManager::GetWeather(
            city);

        Speak(
            "I have checked the weather for " +
            city);

        return true;
    }

    // ==========================================
    // GOOGLE
    // ==========================================

    if (cmd == "google" ||
        cmd == "open google")
    {
        WebManager::OpenGoogle();

        Speak(
            "Opening Google.");

        return true;
    }

    // ==========================================
    // YOUTUBE
    // ==========================================

    if (cmd == "youtube" ||
        cmd == "open youtube")
    {
        WebManager::OpenYouTube();

        Speak(
            "Opening YouTube.");

        return true;
    }

    // ==========================================
    // YOUTUBE SEARCH
    // ==========================================

    if (cmd.find("search youtube ") == 0)
    {
        std::string query =
            After(
                cmd,
                "search youtube ");

        if (query.empty())
        {
            Speak(
                "Please tell me what to search on YouTube.");

            return true;
        }

        WebManager::YouTubeSearch(
            query);

        Speak(
            "Searching YouTube for " +
            query);

        return true;
    }

    // ==========================================
    // GOOGLE SEARCH
    // ==========================================

    if (cmd.find("search google ") == 0)
    {
        std::string query =
            After(
                cmd,
                "search google ");

        if (query.empty())
        {
            Speak(
                "Please tell me what to search on Google.");

            return true;
        }

        WebManager::GoogleSearch(
            query);

        Speak(
            "Searching Google for " +
            query);

        return true;
    }

    // ==========================================
    // GITHUB
    // ==========================================

    if (cmd == "github" ||
        cmd == "open github")
    {
        WebManager::OpenGitHub();

        Speak(
            "Opening GitHub.");

        return true;
    }

    // ==========================================
    // CHATGPT
    // ==========================================

    if (cmd == "chatgpt" ||
        cmd == "open chatgpt")
    {
        WebManager::OpenChatGPT();

        Speak(
            "Opening ChatGPT.");

        return true;
    }

    // ==========================================
    // GMAIL
    // ==========================================

    if (cmd == "gmail" ||
        cmd == "open gmail")
    {
        WebManager::OpenGmail();

        Speak(
            "Opening Gmail.");

        return true;
    }

    // ==========================================
    // LINKEDIN
    // ==========================================

    if (cmd == "linkedin" ||
        cmd == "open linkedin")
    {
        WebManager::OpenLinkedIn();

        Speak(
            "Opening LinkedIn.");

        return true;
    }

    // ==========================================
    // LEETCODE
    // ==========================================

    if (cmd == "leetcode" ||
        cmd == "open leetcode")
    {
        WebManager::OpenLeetCode();

        Speak(
            "Opening LeetCode.");

        return true;
    }

    // ==========================================
    // GEEKSFORGEEKS
    // ==========================================

    if (cmd == "geeksforgeeks" ||
        cmd == "open geeksforgeeks")
    {
        WebManager::OpenGeeksForGeeks();

        Speak(
            "Opening GeeksforGeeks.");

        return true;
    }

    // ==========================================
    // FILE EXPLORER
    // ==========================================

    if (cmd == "open files" ||
        cmd == "open file explorer" ||
        cmd == "file explorer")
    {
        OpenFolder(
            "explorer.exe");

        Speak(
            "Opening file explorer.");

        return true;
    }

    // ==========================================
    // LIST FILES
    // ==========================================

    if (cmd == "list files")
    {
        FileManager::ListFiles();

        Speak(
            "Here are your files.");

        return true;
    }

    // ==========================================
    // CREATE FILE
    // ==========================================

    if (cmd.find("create file ") == 0)
    {
        std::string name =
            After(
                cmd,
                "create file ");

        if (name.empty())
        {
            Speak(
                "Please provide a file name.");

            return true;
        }

        FileManager::CreateFile(
            name);

        Speak(
            "File creation requested.");

        return true;
    }

    // ==========================================
    // CREATE FOLDER
    // ==========================================

    if (cmd.find("create folder ") == 0)
    {
        std::string name =
            After(
                cmd,
                "create folder ");

        if (name.empty())
        {
            Speak(
                "Please provide a folder name.");

            return true;
        }

        FileManager::CreateFolder(
            name);

        Speak(
            "Folder creation requested.");

        return true;
    }

    // ==========================================
    // DELETE FILE
    // ==========================================

    if (cmd.find("delete file ") == 0)
    {
        std::string name =
            After(
                cmd,
                "delete file ");

        if (name.empty())
        {
            Speak(
                "Please provide a file name.");

            return true;
        }

        FileManager::DeleteFile(
            name);

        Speak(
            "File deletion requested.");

        return true;
    }

    // ==========================================
    // DELETE FOLDER
    // ==========================================

    if (cmd.find("delete folder ") == 0)
    {
        std::string name =
            After(
                cmd,
                "delete folder ");

        if (name.empty())
        {
            Speak(
                "Please provide a folder name.");

            return true;
        }

        FileManager::DeleteFolder(
            name);

        Speak(
            "Folder deletion requested.");

        return true;
    }

    // ==========================================
    // RENAME FILE / FOLDER
    // ==========================================

    if (cmd.find("rename ") == 0)
    {
        std::string remaining =
            After(
                cmd,
                "rename ");

        size_t separator =
            remaining.find(" to ");

        if (separator == std::string::npos)
        {
            Speak(
                "Use rename old name to new name.");

            return true;
        }

        std::string oldName =
            remaining.substr(
                0,
                separator);

        std::string newName =
            remaining.substr(
                separator + 4);

        if (oldName.empty() ||
            newName.empty())
        {
            Speak(
                "Please provide both old and new names.");

            return true;
        }

        FileManager::Rename(
            oldName,
            newName);

        Speak(
            "Rename operation requested.");

        return true;
    }

    // ==========================================
    // MUSIC FOLDER
    // ==========================================

    if (cmd == "open music" ||
        cmd == "music")
    {
        OpenFolder(
            "shell:MyMusic");

        Speak(
            "Opening your music folder.");

        return true;
    }

    // ==========================================
    // PLAY MUSIC
    // ==========================================

    if (cmd.find("play ") == 0)
    {
        std::string file =
            After(
                cmd,
                "play ");

        if (file.empty())
        {
            Speak(
                "Please provide a music file.");

            return true;
        }

        MusicManager::Play(
            file);

        Speak(
            "Playing music.");

        return true;
    }

    // ==========================================
    // PAUSE MUSIC
    // ==========================================

    if (cmd == "pause music" ||
        cmd == "pause")
    {
        MusicManager::Pause();

        Speak(
            "Music paused.");

        return true;
    }

    // ==========================================
    // RESUME MUSIC
    // ==========================================

    if (cmd == "resume music" ||
        cmd == "resume")
    {
        MusicManager::Resume();

        Speak(
            "Music resumed.");

        return true;
    }

    // ==========================================
    // STOP MUSIC
    // ==========================================

    if (cmd == "stop music")
    {
        MusicManager::Stop();

        Speak(
            "Music stopped.");

        return true;
    }

    // ==========================================
    // NOTES
    // ==========================================

    if (cmd.find("take note ") == 0)
    {
        std::string note =
            After(
                cmd,
                "take note ");

        if (note.empty())
        {
            Speak(
                "Please tell me what to note.");

            return true;
        }

        NotesManager::AddNote(
            note);

        Speak(
            "Your note has been saved.");

        return true;
    }

    // ==========================================
    // SHOW NOTES
    // ==========================================

    if (cmd == "show notes" ||
        cmd == "show my notes")
    {
        NotesManager::ShowNotes();

        Speak(
            "Here are your notes.");

        return true;
    }

    // ==========================================
    // REMINDERS
    // ==========================================

    if (cmd.find("remind me ") == 0)
    {
        std::string reminder =
            After(
                cmd,
                "remind me ");

        if (reminder.empty())
        {
            Speak(
                "Please tell me what you want to remember.");

            return true;
        }

        ReminderManager::AddReminder(
            reminder);

        Speak(
            "Your reminder has been saved.");

        return true;
    }

    // ==========================================
    // SHOW REMINDERS
    // ==========================================

    if (cmd == "show reminders" ||
        cmd == "show my reminders")
    {
        ReminderManager::ShowReminders();

        Speak(
            "Here are your reminders.");

        return true;
    }

    // ==========================================
    // BATTERY
    // ==========================================

    if (cmd.find("battery") !=
        std::string::npos)
    {
        BatteryManager::ShowBattery();

        Speak(
            "Here is your battery status.");

        return true;
    }

    // ==========================================
    // VOLUME UP
    // ==========================================

    if (cmd == "volume up" ||
        cmd == "increase volume" ||
        cmd == "louder")
    {
        VolumeManager::VolumeUp();

        Speak(
            "Volume increased.");

        return true;
    }

    // ==========================================
    // VOLUME DOWN
    // ==========================================

    if (cmd == "volume down" ||
        cmd == "decrease volume" ||
        cmd == "quieter")
    {
        VolumeManager::VolumeDown();

        Speak(
            "Volume decreased.");

        return true;
    }

    // ==========================================
    // MUTE
    // ==========================================

    if (cmd == "mute" ||
        cmd == "mute volume")
    {
        VolumeManager::Mute();

        Speak(
            "Mute toggled.");

        return true;
    }

    // ==========================================
    // NEWS
    // ==========================================

    if (cmd == "news" ||
        cmd.find("latest news") !=
            std::string::npos)
    {
        NewsManager::GetNews();

        Speak(
            "Here are the latest news headlines.");

        return true;
    }

    // ==========================================
    // SYSTEM INFORMATION
    // ==========================================

    if (cmd == "system information" ||
        cmd == "system info" ||
        cmd == "system status")
    {
        SystemManager::SystemInfo();

        Speak(
            "Here is your system information.");

        return true;
    }

    // ==========================================
    // APP LAUNCHER
    // ==========================================

    if (cmd.find("open app ") == 0)
    {
        std::string app =
            After(
                cmd,
                "open app ");

        if (app.empty())
        {
            Speak(
                "Please provide an application name.");

            return true;
        }

        AppLauncher::Open(
            app);

        Speak(
            "Opening " +
            app);

        return true;
    }

    // ==========================================
    // AI FALLBACK
    // ==========================================

    std::cout
        << "[AI] Thinking...\n";

    std::string response =
        AIManager::Ask(input);

    if (!response.empty())
    {
        Speak(response);
    }
    else
    {
        Speak(
            "I could not get an AI response.");
    }

    return true;
}