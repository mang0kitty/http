#pragma once
#include <chrono>
#include <thread>
#include "httplib.h"
#include "sockstream.h"

class Client
{
  public:
    Client(int socket);
    ~Client();

    void start();
    void disconnect();

  protected:
    bool handleRequest(iostream &stream, HTTPRequest &request);

  private:
    static void runThread(Client *client);

    int socketHandle;
    std::chrono::time_point<std::chrono::system_clock> lastSeen;
    bool keepAlive;
    bool connected;
    std::thread *thread = NULL;
};