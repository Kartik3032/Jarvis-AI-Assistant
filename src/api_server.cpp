#include "../include/api_server.h"
#include "../include/command.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

namespace
{
    SOCKET serverSocket = INVALID_SOCKET;
    bool running = false;
    std::thread serverThread;

    std::string ExtractBody(const std::string &request)
    {
        size_t pos = request.find("\r\n\r\n");

        if (pos == std::string::npos)
            return "";

        return request.substr(pos + 4);
    }

    std::string JsonUnescape(const std::string &input)
    {
        std::string result;

        for (size_t i = 0; i < input.size(); ++i)
        {
            if (input[i] == '\\' && i + 1 < input.size())
            {
                if (input[i + 1] == '"')
                {
                    result += '"';
                    ++i;
                }
                else if (input[i + 1] == '\\')
                {
                    result += '\\';
                    ++i;
                }
                else
                {
                    result += input[i];
                }
            }
            else
            {
                result += input[i];
            }
        }

        return result;
    }

    std::string ExtractCommand(const std::string &body)
    {
        std::string key = "\"command\"";

        size_t keyPos = body.find(key);

        if (keyPos == std::string::npos)
            return "";

        size_t colon = body.find(':', keyPos);

        if (colon == std::string::npos)
            return "";

        size_t firstQuote = body.find('"', colon);

        if (firstQuote == std::string::npos)
            return "";

        size_t secondQuote = firstQuote + 1;

        while (secondQuote < body.size())
        {
            if (body[secondQuote] == '"' &&
                body[secondQuote - 1] != '\\')
            {
                break;
            }

            ++secondQuote;
        }

        if (secondQuote >= body.size())
            return "";

        return JsonUnescape(
            body.substr(
                firstQuote + 1,
                secondQuote - firstQuote - 1));
    }

    std::string MakeResponse(
        const std::string &message,
        bool success)
    {
        std::string safe = message;

        size_t pos = 0;

        while ((pos = safe.find('\\', pos)) != std::string::npos)
        {
            safe.replace(pos, 1, "\\\\");
            pos += 2;
        }

        pos = 0;

        while ((pos = safe.find('"', pos)) != std::string::npos)
        {
            safe.replace(pos, 1, "\\\"");
            pos += 2;
        }

        return "{\"success\":" +
               std::string(success ? "true" : "false") +
               ",\"message\":\"" +
               safe +
               "\"}";
    }

    void SendResponse(
        SOCKET client,
        const std::string &body)
    {
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n"
            "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
            "Content-Length: " +
            std::to_string(body.size()) +
            "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            body;

        send(
            client,
            response.c_str(),
            static_cast<int>(response.size()),
            0);
    }

    void HandleClient(SOCKET client)
    {
        char buffer[16384]{};

        int received =
            recv(
                client,
                buffer,
                sizeof(buffer) - 1,
                0);

        if (received <= 0)
        {
            closesocket(client);
            return;
        }

        std::string request(
            buffer,
            received);

        if (request.find("OPTIONS") == 0)
        {
            SendResponse(
                client,
                "{\"success\":true}");
        }
        else if (request.find("POST /api/command") == 0)
        {
            std::string body =
                ExtractBody(request);

            std::string command =
                ExtractCommand(body);

            std::cout
                << "\n[UI COMMAND] "
                << command
                << "\n";

            if (command.empty())
            {
                SendResponse(
                    client,
                    MakeResponse(
                        "No command received.",
                        false));
            }
            else
            {
                bool result =
                    Command::ProcessCommand(command);

                std::string message;

                if (result)
                {
                    message =
                        "Command executed successfully.";
                }
                else
                {
                    message =
                        "Command processed.";
                }

                SendResponse(
                    client,
                    MakeResponse(
                        message,
                        true));
            }
        }
        else if (request.find("GET /api/status") == 0)
        {
            SendResponse(
                client,
                "{\"success\":true,\"message\":\"JARVIS ONLINE\"}");
        }
        else
        {
            SendResponse(
                client,
                MakeResponse(
                    "Unknown API endpoint.",
                    false));
        }

        closesocket(client);
    }

    void ServerLoop(int port)
    {
        WSADATA wsa{};

        if (WSAStartup(
                MAKEWORD(2, 2),
                &wsa) != 0)
        {
            std::cout
                << "WSAStartup failed.\n";

            running = false;
            return;
        }

        serverSocket =
            socket(
                AF_INET,
                SOCK_STREAM,
                IPPROTO_TCP);

        if (serverSocket == INVALID_SOCKET)
        {
            std::cout
                << "Socket creation failed.\n";

            WSACleanup();
            running = false;
            return;
        }

        sockaddr_in address{};

        address.sin_family = AF_INET;
        address.sin_addr.s_addr =
            htonl(INADDR_ANY);
        address.sin_port =
            htons(static_cast<u_short>(port));

        if (bind(
                serverSocket,
                reinterpret_cast<sockaddr *>(&address),
                sizeof(address)) == SOCKET_ERROR)
        {
            std::cout
                << "Bind failed. Port "
                << port
                << " may already be in use.\n";

            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;

            WSACleanup();

            running = false;
            return;
        }

        if (listen(serverSocket, 10) == SOCKET_ERROR)
        {
            std::cout
                << "Listen failed.\n";

            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;

            WSACleanup();

            running = false;
            return;
        }

        std::cout
            << "\n=================================\n"
            << "       JARVIS API SERVER\n"
            << "=================================\n"
            << "http://localhost:"
            << port
            << "\n\n";

        while (running)
        {
            SOCKET client =
                accept(
                    serverSocket,
                    nullptr,
                    nullptr);

            if (client == INVALID_SOCKET)
            {
                if (running)
                    std::cout << "Accept failed.\n";

                break;
            }

            std::thread(
                HandleClient,
                client)
                .detach();
        }

        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;

        WSACleanup();
    }
}

bool APIServer::Start(int port)
{
    if (running)
        return true;

    running = true;

    serverThread =
        std::thread(
            ServerLoop,
            port);

    return true;
}

void APIServer::Stop()
{
    if (!running)
        return;

    running = false;

    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    if (serverThread.joinable())
        serverThread.join();
}