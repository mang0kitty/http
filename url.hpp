#pragma once
#include <string>

using namespace std;

class Url
{
  public:
    Url(string url);
    ~Url();

    string getHostname();
    string getProtocol();
    int getPort();
    string getPath();

  private:
    string hostname;
    string protocol;
    int port;
    string path;
};