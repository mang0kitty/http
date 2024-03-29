#include "client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <sstream>

Client::Client(int socket)
{
    this->connected = true;
    this->socketHandle = socket;
    this->lastSeen = std::chrono::system_clock::now();
    this->keepAlive = true;
}

Client::~Client()
{
    this->disconnect();

    if (this->thread != NULL)
    {
        this->thread->join();
    }
}

void Client::start()
{
    this->thread = new std::thread(Client::runThread, this);
}

void Client::disconnect()
{
    if (!this->connected)
        return;

    this->connected = false;
    shutdown(this->socketHandle, SHUT_RDWR);
}

bool Client::handleRequest(iostream &stream, HTTPRequest &request)
{
    this->keepAlive = request.getHeader("Connection", "close").compare("keep-alive") == 0;

    if (request.getMethod().compare("GET") == 0)
    {
        handleGetRequest(stream, request);
    }
    else
    {
        HTTPResponse response(405, "Method Not Allowed");
        response.setHeader("Connection", request.getHeader("Connection", "close"));
        response.setContent("Thanks for your request, we don't yet support this method.");

        response.serialize(stream);
        stream.flush();
    }

    return true;
}

void Client::handleGetRequest(iostream &stream, HTTPRequest &request)
{

    ifstream fileStream("./" + request.getPath());
    stringstream fileContent;

    bool fileFound = false;

    char c = fileStream.get();
    while (fileStream.good())
    {
        fileFound = true;

        fileContent << c;
        c = fileStream.get();
    }

    fileStream.close();

    if (fileFound)
    {

        HTTPResponse response(200, "OK");
        response.setHeader("Connection", request.getHeader("Connection", "close"));
        response.setContent(fileContent.str());

        response.serialize(stream);
    }
    else
    {

        HTTPResponse response(404, "Not Found");
        response.setHeader("Connection", request.getHeader("Connection", "close"));
        response.setContent("Sorry, we couldn't find the file you were looking for.");

        response.serialize(stream);
    }
    stream.flush();
}

void Client::handlePostRequest(iostream &stream, HTTPRequest &request)
{
}

void Client::runThread(Client *client)
{
    iosockstreambuf streamBuf(client->socketHandle, 1024);
    iostream stream(&streamBuf);

    while (client->connected)
    {
        HTTPRequest request;
        if (!request.deserialize(stream))
        {
            break;
        }

        client->lastSeen = std::chrono::system_clock::now();

        if (!client->handleRequest(stream, request))
        {
            break;
        }

        if (!client->keepAlive)
        {
            break;
        }
    }

    client->disconnect();
}