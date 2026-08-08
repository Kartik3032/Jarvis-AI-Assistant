#include "../include/news_manager.h"

#include <windows.h>
#include <winhttp.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#pragma comment(lib, "winhttp.lib")

void NewsManager::GetNews()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const char *apiKey = std::getenv("NEWS_API_KEY");

    if (apiKey == nullptr)
    {
        std::cout << "News API key not configured\n";
        return;
    }

    std::wstring path =
        L"/v2/everything?q=India&sortBy=publishedAt&pageSize=5&apiKey=" +
        std::wstring(apiKey, apiKey + strlen(apiKey));

    HINTERNET session = WinHttpOpen(
        L"JarvisNews/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (!session)
    {
        std::cout << "Unable to initialize HTTP session\n";
        return;
    }

    HINTERNET connection = WinHttpConnect(
        session,
        L"newsapi.org",
        INTERNET_DEFAULT_HTTPS_PORT,
        0);

    if (!connection)
    {
        std::cout << "Unable to connect to news server\n";
        WinHttpCloseHandle(session);
        return;
    }

    HINTERNET request = WinHttpOpenRequest(
        connection,
        L"GET",
        path.c_str(),
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);

    if (!request)
    {
        std::cout << "Unable to create news request\n";

        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return;
    }

    BOOL result = WinHttpSendRequest(
        request,
        WINHTTP_NO_ADDITIONAL_HEADERS,
        0,
        WINHTTP_NO_REQUEST_DATA,
        0,
        0,
        0);

    if (result)
    {
        result = WinHttpReceiveResponse(
            request,
            NULL);
    }

    if (!result)
    {
        std::cout << "News request failed\n";

        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return;
    }

    std::string response;

    DWORD available = 0;

    do
    {
        available = 0;

        if (!WinHttpQueryDataAvailable(
                request,
                &available))
        {
            break;
        }

        if (available == 0)
        {
            break;
        }

        std::string buffer(
            available,
            '\0');

        DWORD downloaded = 0;

        if (!WinHttpReadData(
                request,
                buffer.data(),
                available,
                &downloaded))
        {
            break;
        }

        response.append(
            buffer,
            0,
            downloaded);

    } while (available > 0);

    std::cout << "\n========== LATEST NEWS ==========\n\n";

    size_t articlesPos = response.find("\"articles\":[");

    if (articlesPos == std::string::npos)
    {
        std::cout << "No news articles found.\n";
    }
    else
    {
        size_t position = articlesPos;

        for (int i = 1; i <= 5; i++)
        {
            size_t titlePos =
                response.find("\"title\":\"", position);

            if (titlePos == std::string::npos)
                break;

            titlePos += 9;

            size_t titleEnd =
                response.find("\"", titlePos);

            if (titleEnd == std::string::npos)
                break;

            std::string title =
                response.substr(
                    titlePos,
                    titleEnd - titlePos);

            size_t sourcePos =
                response.find("\"name\":\"", position);

            std::string source = "Unknown";

            if (sourcePos != std::string::npos)
            {
                sourcePos += 8;

                size_t sourceEnd =
                    response.find("\"", sourcePos);

                if (sourceEnd != std::string::npos)
                {
                    source =
                        response.substr(
                            sourcePos,
                            sourceEnd - sourcePos);
                }
            }

            std::cout << i << ". " << title << "\n";
            std::cout << "   Source: " << source << "\n\n";

            position = titleEnd + 1;
        }
    }

    std::cout << "=================================\n";
}