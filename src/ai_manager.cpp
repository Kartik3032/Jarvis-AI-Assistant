#include "../include/ai_manager.h"

#include <windows.h>
#include <winhttp.h>

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#pragma comment(lib, "winhttp.lib")

namespace
{
    bool g_initialized = false;

    std::wstring g_apiKey;

    const wchar_t *HOST =
        L"generativelanguage.googleapis.com";

    const wchar_t *PATH =
        L"/v1beta/models/gemini-3.6-flash:generateContent";

    std::vector<std::pair<std::string, std::string>>
        g_history;

    std::string JsonEscape(
        const std::string &input)
    {
        std::string result;

        for (char c : input)
        {
            switch (c)
            {
            case '\"':
                result += "\\\"";
                break;

            case '\\':
                result += "\\\\";
                break;

            case '\n':
                result += "\\n";
                break;

            case '\r':
                result += "\\r";
                break;

            case '\t':
                result += "\\t";
                break;

            default:
                result += c;
                break;
            }
        }

        return result;
    }

    std::string ExtractJsonString(
        const std::string &json,
        const std::string &key)
    {
        std::string search =
            "\"" + key + "\"";

        size_t pos =
            json.find(search);

        if (pos == std::string::npos)
            return "";

        pos =
            json.find(':', pos);

        if (pos == std::string::npos)
            return "";

        pos++;

        while (
            pos < json.size() &&
            (json[pos] == ' ' ||
             json[pos] == '\n' ||
             json[pos] == '\r' ||
             json[pos] == '\t'))
        {
            pos++;
        }

        if (pos >= json.size() ||
            json[pos] != '"')
        {
            return "";
        }

        pos++;

        std::string result;
        bool escaped = false;

        for (; pos < json.size(); ++pos)
        {
            char c = json[pos];

            if (escaped)
            {
                switch (c)
                {
                case 'n':
                    result += '\n';
                    break;

                case 'r':
                    result += '\r';
                    break;

                case 't':
                    result += '\t';
                    break;

                case '"':
                    result += '"';
                    break;

                case '\\':
                    result += '\\';
                    break;

                default:
                    result += c;
                    break;
                }

                escaped = false;
            }
            else if (c == '\\')
            {
                escaped = true;
            }
            else if (c == '"')
            {
                break;
            }
            else
            {
                result += c;
            }
        }

        return result;
    }

    std::string BuildRequest(
        const std::string &message)
    {
        std::string json;

        json += "{";

        // System instruction
        json +=
            "\"system_instruction\":{"
            "\"parts\":[{"
            "\"text\":\"";

        json += JsonEscape(
            "You are Jarvis, a personal desktop AI assistant. "
            "Be concise, friendly and natural. "
            "The user is speaking to you through voice. "
            "Do not use markdown unless necessary. "
            "Keep normal answers short enough to be spoken aloud.");

        json +=
            "\"}]"
            "},";

        // Conversation history
        json += "\"contents\":[";

        for (const auto &item : g_history)
        {
            json += "{";

            json +=
                "\"role\":\"" +
                item.first +
                "\",";

            json +=
                "\"parts\":[{\"text\":\"" +
                JsonEscape(item.second) +
                "\"}]";

            json += "},";
        }

        // Current message
        json +=
            "{"
            "\"role\":\"user\","
            "\"parts\":[{\"text\":\"";

        json += JsonEscape(message);

        json +=
            "\"}]"
            "}";

        json += "],";

        json +=
            "\"generationConfig\":{"
            "\"temperature\":0.7,"
            "\"maxOutputTokens\":300"
            "}";

        json += "}";

        return json;
    }

    std::string HttpPost(
        const std::string &body)
    {
        HINTERNET session =
            WinHttpOpen(
                L"JarvisAI/1.0",
                WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                WINHTTP_NO_PROXY_NAME,
                WINHTTP_NO_PROXY_BYPASS,
                0);

        if (!session)
            return "";

        HINTERNET connection =
            WinHttpConnect(
                session,
                HOST,
                INTERNET_DEFAULT_HTTPS_PORT,
                0);

        if (!connection)
        {
            WinHttpCloseHandle(session);
            return "";
        }

        HINTERNET request =
            WinHttpOpenRequest(
                connection,
                L"POST",
                PATH,
                nullptr,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                WINHTTP_FLAG_SECURE);

        if (!request)
        {
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(session);

            return "";
        }

        std::wstring headers =
            L"Content-Type: application/json\r\n"
            L"x-goog-api-key: " +
            g_apiKey +
            L"\r\n";

        BOOL sent =
            WinHttpSendRequest(
                request,
                headers.c_str(),
                static_cast<DWORD>(-1L),
                (LPVOID)body.data(),
                static_cast<DWORD>(body.size()),
                static_cast<DWORD>(body.size()),
                0);

        if (!sent)
        {
            WinHttpCloseHandle(request);
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(session);

            return "";
        }

        BOOL received =
            WinHttpReceiveResponse(
                request,
                nullptr);

        if (!received)
        {
            WinHttpCloseHandle(request);
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(session);

            return "";
        }

        std::string response;

        DWORD available = 0;

        while (
            WinHttpQueryDataAvailable(
                request,
                &available) &&
            available > 0)
        {
            std::vector<char> buffer(
                available + 1);

            DWORD downloaded = 0;

            if (!WinHttpReadData(
                    request,
                    buffer.data(),
                    available,
                    &downloaded))
            {
                break;
            }

            buffer[downloaded] = '\0';

            response += buffer.data();
        }

        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return response;
    }
}

bool AIManager::Initialize()
{
    char key[4096];

    DWORD length =
        GetEnvironmentVariableA(
            "GEMINI_API_KEY",
            key,
            sizeof(key));

    if (length == 0 ||
        length >= sizeof(key))
    {
        std::cout
            << "[AI] GEMINI_API_KEY not found.\n";

        g_initialized = false;

        return false;
    }

    int size =
        MultiByteToWideChar(
            CP_UTF8,
            0,
            key,
            -1,
            nullptr,
            0);

    if (size <= 0)
        return false;

    std::wstring wideKey(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        key,
        -1,
        wideKey.data(),
        size);

    g_apiKey = wideKey;

    g_initialized = true;

    std::cout
        << "[AI] Gemini connected.\n";

    return true;
}

std::string AIManager::Chat(
    const std::string &message)
{
    if (!g_initialized)
    {
        return "My AI connection is not configured yet.";
    }

    std::string body =
        BuildRequest(message);

    std::string response =
        HttpPost(body);

    if (response.empty())
    {
        return "I could not connect to my AI service.";
    }

    std::string answer =
        ExtractJsonString(
            response,
            "text");

    if (answer.empty())
    {
        std::cout
            << "[AI] Invalid response:\n"
            << response
            << "\n";

        return "I received an invalid response from the AI service.";
    }

    // Store conversation
    g_history.push_back(
        {"user", message});

    g_history.push_back(
        {"model", answer});

    // Keep last 10 messages
    while (g_history.size() > 10)
    {
        g_history.erase(
            g_history.begin());

        g_history.erase(
            g_history.begin());
    }

    return answer;
}

void AIManager::ClearHistory()
{
    g_history.clear();
}