#pragma once

#include <string>

class Config
{
public:
    static bool Load();

    static std::string GetAssistantName();

    static std::string GetLanguage();

    static std::string GetVoice();

private:
    static std::string assistantName;
    static std::string language;
    static std::string voice;
};