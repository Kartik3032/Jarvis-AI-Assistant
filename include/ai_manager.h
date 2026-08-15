#pragma once

#include <string>

class AIManager
{
public:
    static bool Initialize();
    static std::string Chat(const std::string &message);
    static void ClearHistory();
};