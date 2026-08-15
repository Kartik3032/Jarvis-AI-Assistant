#pragma once

#include <string>

class Command
{
public:
    static bool ProcessCommand(
        const std::string &command);
};