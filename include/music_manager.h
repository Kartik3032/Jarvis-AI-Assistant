#pragma once

#include <string>

class MusicManager
{
public:
    static void Play(const std::string &filePath);
    static void Pause();
    static void Resume();
    static void Stop();
};