#pragma once
#include <chrono>
#include <thread>
#include "httplib.h"
#include "sockstream.h"
#include "fileprovider.hpp"

class Client
{
public:
  Client(int socket, FileProvider *fileProvider);
  ~Client();

  void start();
  void disconnect();

protected:
  bool handleRequest(iostream &stream, HTTPRequest &request);

  void handleGetRequest(iostream &stream, HTTPRequest &request);
  void handlePostRequest(iostream &stream, HTTPRequest &request);

private:
  static void runThread(Client *client);

  int socketHandle;
  FileProvider *fileProvider;
  std::chrono::time_point<std::chrono::system_clock> lastSeen;
  bool keepAlive;
  bool connected;
  std::thread *thread = NULL;
};