#include "../include/music_manager.h"

#include <windows.h>
#include <mmsystem.h>

#include <iostream>
#include <string>

#pragma comment(lib, "winmm.lib")

void MusicManager::Play(const std::string &filePath)
{
    std::string cleanPath = filePath;

    // Remove surrounding quotes
    if (cleanPath.size() >= 2 &&
        cleanPath.front() == '"' &&
        cleanPath.back() == '"')
    {
        cleanPath =
            cleanPath.substr(1, cleanPath.size() - 2);
    }

    // Close previous song
    mciSendStringA(
        "close jarvisMusic",
        NULL,
        0,
        NULL);

    std::string command =
        "open \"" +
        cleanPath +
        "\" type mpegvideo alias jarvisMusic";

    MCIERROR error = mciSendStringA(
        command.c_str(),
        NULL,
        0,
        NULL);

    if (error != 0)
    {
        char errorText[256]{};

        mciGetErrorStringA(
            error,
            errorText,
            sizeof(errorText));

        std::cout << "Unable to open music file\n";
        std::cout << "MCI Error: "
                  << errorText
                  << "\n";

        return;
    }

    error = mciSendStringA(
        "play jarvisMusic",
        NULL,
        0,
        NULL);

    if (error != 0)
    {
        char errorText[256]{};

        mciGetErrorStringA(
            error,
            errorText,
            sizeof(errorText));

        std::cout << "Unable to play music\n";
        std::cout << "MCI Error: "
                  << errorText
                  << "\n";

        return;
    }

    std::cout << "Playing: "
              << cleanPath
              << "\n";
}

void MusicManager::Pause()
{
    mciSendStringA(
        "pause jarvisMusic",
        NULL,
        0,
        NULL);

    std::cout << "Music paused\n";
}

void MusicManager::Resume()
{
    mciSendStringA(
        "resume jarvisMusic",
        NULL,
        0,
        NULL);

    std::cout << "Music resumed\n";
}

void MusicManager::Stop()
{
    mciSendStringA(
        "stop jarvisMusic",
        NULL,
        0,
        NULL);

    mciSendStringA(
        "close jarvisMusic",
        NULL,
        0,
        NULL);

    std::cout << "Music stopped\n";
}