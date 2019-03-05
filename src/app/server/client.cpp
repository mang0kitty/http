#include "client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

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
    HTTPResponse response(200, "OK");
    response.setHeader("Connection", request.getHeader("Connection", "close"));
    response.setContent("Thanks for your request!");

    response.serialize(stream);
    stream.flush();

    this->keepAlive = request.getHeader("Connection", "close").compare("keep-alive") == 0;

    return true;
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