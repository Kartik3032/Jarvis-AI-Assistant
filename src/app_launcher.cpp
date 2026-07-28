#include "../include/app_launcher.h"

#include <iostream>
#include <cstdlib>

void AppLauncher::Open(const std::string &app)
{
    if (app == "chrome")
    {
        system("start chrome");
    }
    else if (app == "calculator")
    {
        system("start calc");
    }
    else if (app == "notepad")
    {
        system("start notepad");
    }
    else if (app == "youtube")
    {
        system("start https://youtube.com");
    }
    else
    {
        std::cout << "Unknown application\n";
    }
}