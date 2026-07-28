#pragma once

#include <string>

struct ParsedCommand
{
    std::string action;
    std::string target;
    std::string argument;
};

class Parser
{
public:
    static ParsedCommand Parse(const std::string &input);
};