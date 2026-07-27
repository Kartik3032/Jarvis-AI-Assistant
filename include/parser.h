#pragma once

#include <string>

struct ParsedCommand
{
    std::string action;
    std::string target;
};

class Parser
{
public:
    static ParsedCommand Parse(const std::string &input);
};