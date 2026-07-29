#pragma once

#include <string>

class WebManager
{
public:
    // Search
    static void GoogleSearch(const std::string &);
    static void YouTubeSearch(const std::string &);
    static void GitHubSearch(const std::string &);
    static void StackOverflowSearch(const std::string &);
    static void RedditSearch(const std::string &);
    static void WikipediaSearch(const std::string &);

    // Websites
    static void OpenGoogle();
    static void OpenYouTube();
    static void OpenGitHub();
    static void OpenGmail();
    static void OpenChatGPT();
    static void OpenLinkedIn();
    static void OpenLeetCode();
    static void OpenGeeksForGeeks();
};