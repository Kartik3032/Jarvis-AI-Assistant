#include "../include/ai_manager.h"

#include <windows.h>
#include <winhttp.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "winhttp.lib")

namespace
{
    std::wstring g_apiKey;

    bool g_ready = false;

    const wchar_t *HOST =
        L"generativelanguage.googleapis.com";

    const wchar_t *PATH =
        L"/v1beta/models/gemini-3.7-flash:generateContent";

    std::string EscapeJson(
        const std::string &text)
    {
        std::string result;

        for (char c : text)
        {
            switch (c)
            {
            case '"':
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

    std::string ExtractText(
        const std::string &json)
    {
        const std::string marker =
            "\"text\"";

        size_t pos =
            json.find(marker);

        if (pos == std::string::npos)
            return "";

        pos =
            json.find(':', pos);

        if (pos == std::string::npos)
            return "";

        pos++;

        while (
            pos < json.size() &&
            std::isspace(
                static_cast<unsigned char>(
                    json[pos])))
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

        while (pos < json.size())
        {
            char c = json[pos++];

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

                continue;
            }

            if (c == '\\')
            {
                escaped = true;
                continue;
            }

            if (c == '"')
                break;

            result += c;
        }

        return result;
    }

    std::string BuildJson(
        const std::string &prompt)
    {
        std::string json;

        json =
            "{"
            "\"system_instruction\":{"
            "\"parts\":[{"
            "\"text\":\"";

        json += EscapeJson(
            "You are Jarvis, a concise and friendly "
            "desktop AI assistant. "
            "The user speaks through voice. "
            "Give short natural answers suitable "
            "for text-to-speech. "
            "Do not use markdown unless necessary.");

        json +=
            "\"}]"
            "},"
            "\"contents\":[{"
            "\"role\":\"user\","
            "\"parts\":[{"
            "\"text\":\"";

        json += EscapeJson(prompt);

        json +=
            "\"}]"
            "}]"
            "}";

        return json;
    }

    std::string SendRequest(
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

        BOOL ok =
            WinHttpSendRequest(
                request,
                headers.c_str(),
                static_cast<DWORD>(-1L),
                (LPVOID)body.data(),
                static_cast<DWORD>(body.size()),
                static_cast<DWORD>(body.size()),
                0);

        if (!ok)
        {
            WinHttpCloseHandle(request);
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(session);

            return "";
        }

        ok =
            WinHttpReceiveResponse(
                request,
                nullptr);

        if (!ok)
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

            response.append(
                buffer.data(),
                downloaded);
        }

        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return response;
    }
}

bool AIManager::Initialize()
{
    const char *key =
        std::getenv("GEMINI_API_KEY");

    if (!key || std::string(key).empty())
    {
        std::cout
            << "[AI] GEMINI_API_KEY not found.\n";

        g_ready = false;

        return false;
    }

    int length =
        MultiByteToWideChar(
            CP_UTF8,
            0,
            key,
            -1,
            nullptr,
            0);

    if (length <= 0)
    {
        g_ready = false;
        return false;
    }

    g_apiKey.resize(length);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        key,
        -1,
        g_apiKey.data(),
        length);

    g_ready = true;

    std::cout
        << "[AI] Gemini connected.\n";

    return true;
}

std::string AIManager::Ask(
    const std::string &prompt)
{
    if (!g_ready)
        return "";

    std::string body =
        BuildJson(prompt);

    std::string response =
        SendRequest(body);

    if (response.empty())
    {
        std::cout
            << "[AI] Empty response.\n";

        return "";
    }

    std::string answer =
        ExtractText(response);

    if (answer.empty())
    {
        std::cout
            << "[AI] Could not parse response.\n";

        return "";
    }

    return answer;
}