#include "../include/ai_manager.h"

#include <cstdlib>
#include <iostream>

bool AIManager::Initialize()
{
    const char *key =
        std::getenv("GEMINI_API_KEY");

    if (!key || std::string(key).empty())
    {
        std::cout
            << "[AI] GEMINI_API_KEY not found.\n";

        return false;
    }

    std::cout
        << "[AI] Gemini configured.\n";

    return true;
}

std::string AIManager::Ask(
    const std::string &prompt)
{
    const char *key =
        std::getenv("GEMINI_API_KEY");

    if (!key || std::string(key).empty())
        return "";

    /*
        Gemini HTTP request goes here.

        Keep API key outside source code.
    */

    std::cout
        << "[AI] Query: "
        << prompt
        << "\n";

    return "";
}