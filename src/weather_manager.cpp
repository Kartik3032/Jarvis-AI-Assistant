#include "../include/weather_manager.h"

#include <windows.h>
#include <winhttp.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#pragma comment(lib, "winhttp.lib")

// JSON helper — GetWeather ke BAHAR
std::string GetJsonValue(
    const std::string &json,
    const std::string &key)
{
    std::string searchKey = "\"" + key + "\":";

    size_t start = json.find(searchKey);

    if (start == std::string::npos)
        return "N/A";

    start += searchKey.length();

    while (start < json.length() &&
           (json[start] == ' ' || json[start] == '"'))
    {
        start++;
    }

    size_t end = start;

    while (end < json.length() &&
           json[end] != ',' &&
           json[end] != '}' &&
           json[end] != '"')
    {
        end++;
    }

    return json.substr(start, end - start);
}

// Actual Weather function
void WeatherManager::GetWeather(const std::string &city)
{
    const char *apiKey = std::getenv("OPENWEATHER_API_KEY");

    if (apiKey == nullptr)
    {
        std::cout << "Weather API key not configured\n";
        return;
    }

    std::wstring wideCity(city.begin(), city.end());

    std::wstring path =
        L"/data/2.5/weather?q=" +
        wideCity +
        L"&appid=" +
        std::wstring(apiKey, apiKey + strlen(apiKey)) +
        L"&units=metric";

    HINTERNET session = WinHttpOpen(
        L"JarvisWeather/1.0",
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
        L"api.openweathermap.org",
        INTERNET_DEFAULT_HTTPS_PORT,
        0);

    if (!connection)
    {
        std::cout << "Unable to connect to weather server\n";
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
        std::cout << "Unable to create weather request\n";
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
        result = WinHttpReceiveResponse(request, NULL);
    }

    if (!result)
    {
        std::cout << "Weather request failed\n";

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
            break;

        std::string buffer(available, '\0');

        DWORD downloaded = 0;

        if (!WinHttpReadData(
                request,
                buffer.data(),
                available,
                &downloaded))
        {
            break;
        }

        response.append(buffer, 0, downloaded);

    } while (available > 0);

    // Clean weather output
    std::string temperature =
        GetJsonValue(response, "temp");

    std::string feelsLike =
        GetJsonValue(response, "feels_like");

    std::string humidity =
        GetJsonValue(response, "humidity");

    std::string cityName =
        GetJsonValue(response, "name");

    std::cout << "\n========== WEATHER ==========\n";

    std::cout << "City        : "
              << cityName << "\n";

    std::cout << "Temperature : "
              << temperature << " °C\n";

    std::cout << "Feels Like  : "
              << feelsLike << " °C\n";

    std::cout << "Humidity    : "
              << humidity << " %\n";

    std::cout << "=============================\n";

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connection);
    WinHttpCloseHandle(session);
}