#include "../include/parser.h"

#include <sstream>

ParsedCommand Parser::Parse(const std::string &input)
{
    ParsedCommand cmd;

    std::stringstream ss(input);

    ss >> cmd.action;
    ss >> cmd.target;

    return cmd;
}