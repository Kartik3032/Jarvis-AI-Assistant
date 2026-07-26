#pragma once

#include <string>

class Command
{
public:
    static std::string GetCommand();
    static bool ProcessCommand(const std::string &command);
};