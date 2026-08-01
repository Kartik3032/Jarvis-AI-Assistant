#pragma once

#include <string>
#include <vector>

class Command
{
public:
    static std::string GetCommand();
    static bool ProcessCommand(const std::string &command);

private:
    static std::vector<std::string> history;
};