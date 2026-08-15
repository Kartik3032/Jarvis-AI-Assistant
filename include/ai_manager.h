#pragma once

#include <string>

class AIManager
{
public:
    static bool Initialize();

    static std::string Ask(
        const std::string &prompt);
};