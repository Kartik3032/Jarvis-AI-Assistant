#include "../include/config.h"

std::string Config::assistantName = "Jarvis";
std::string Config::language = "en-IN";
std::string Config::voice = "Male";

bool Config::Load()
{
    return true;
}

std::string Config::GetAssistantName()
{
    return assistantName;
}

std::string Config::GetLanguage()
{
    return language;
}

std::string Config::GetVoice()
{
    return voice;
}