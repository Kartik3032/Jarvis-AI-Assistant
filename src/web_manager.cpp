#include "../include/web_manager.h"

#include <windows.h>
#include <string>

//--------------------------------------------
// Helper Function
//--------------------------------------------

void OpenURL(const std::string &url)
{
    ShellExecuteA(
        NULL,
        "open",
        url.c_str(),
        NULL,
        NULL,
        SW_SHOWNORMAL);
}

//--------------------------------------------
// Search Functions
//--------------------------------------------

void WebManager::GoogleSearch(const std::string &query)
{
    OpenURL("https://www.google.com/search?q=" + query);
}

void WebManager::YouTubeSearch(const std::string &query)
{
    OpenURL("https://www.youtube.com/results?search_query=" + query);
}

void WebManager::GitHubSearch(const std::string &query)
{
    OpenURL("https://github.com/search?q=" + query);
}

void WebManager::StackOverflowSearch(const std::string &query)
{
    OpenURL("https://stackoverflow.com/search?q=" + query);
}

void WebManager::RedditSearch(const std::string &query)
{
    OpenURL("https://www.reddit.com/search/?q=" + query);
}

void WebManager::WikipediaSearch(const std::string &query)
{
    OpenURL("https://en.wikipedia.org/wiki/" + query);
}

//--------------------------------------------
// Direct Websites
//--------------------------------------------

void WebManager::OpenGoogle()
{
    OpenURL("https://www.google.com");
}

void WebManager::OpenYouTube()
{
    OpenURL("https://www.youtube.com");
}

void WebManager::OpenGitHub()
{
    OpenURL("https://github.com");
}

void WebManager::OpenGmail()
{
    OpenURL("https://mail.google.com");
}

void WebManager::OpenChatGPT()
{
    OpenURL("https://chatgpt.com");
}

void WebManager::OpenLinkedIn()
{
    OpenURL("https://www.linkedin.com");
}

void WebManager::OpenLeetCode()
{
    OpenURL("https://leetcode.com");
}

void WebManager::OpenGeeksForGeeks()
{
    OpenURL("https://www.geeksforgeeks.org");
}