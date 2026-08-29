#pragma once

class APIServer
{
public:
    static bool Start(int port = 8080);
    static void Stop();
};