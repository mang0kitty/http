#include "url.hpp"

Url::Url(string url)
{
    if (url.find("://") != std::string::npos)
    {
        protocol = url.substr(0, url.find("://"));

        url = url.substr(url.find("://") + 3);
    }
    else
    {
        protocol = "http";
    }

    if (url.find(':') != std::string::npos)
    {
        hostname = url.substr(0, url.find(':'));

        url = url.substr(url.find(":"));
    }
    else
    {
        hostname = url.substr(0, url.find('/'));
        url = url.substr(url.find("/"));
    }

    if (url[0] == ':')
    {
        port = stoi(url.substr(1, url.find("/")));
        url = url.substr(url.find("/"));
    }
    else
    {
        port = 80;
    }

    path = url;
    if (path.size() == 0)
    {
        path = "/";
    }
}

Url::~Url()
{
}

string Url::getProtocol()
{
    return this->protocol;
}

string Url::getHostname()
{
    return this->hostname;
}

int Url::getPort()
{
    return this->port;
}

string Url::getPath()
{
    return this->path;
}
