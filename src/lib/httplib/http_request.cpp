#include "HTTP_Request.hpp"

HTTPRequest::HTTPRequest() : HTTPMessage()
{
    this->setMethod("GET");
    this->setPath("/");
}

HTTPRequest::HTTPRequest(string method, string path) : HTTPMessage()
{
    this->setMethod(method);
    this->setPath(path);
}

HTTPRequest::HTTPRequest(string method, string path, vector<uint8_t> content) : HTTPMessage()
{
    this->setMethod(method);
    this->setPath(path);
    this->setContent(content);
}

void HTTPRequest::setMethod(string method)
{
    this->method = method;
}

string HTTPRequest::getMethod()
{
    return this->method;
}

void HTTPRequest::setPath(string path)
{
    this->path = path;
}

string HTTPRequest::getPath()
{
    return this->path;
}

void HTTPRequest::serialize(ostream &into)
{
    into << this->getMethod() << " " << this->getPath() << " HTTP/" << this->getVersion() << "\r\n";

    this->serializeHeadersAndContent(into);
}

bool HTTPRequest::deserialize(istream &from)
{
    string firstLine;
    if (!getline(from, firstLine))
    {
        return false;
    }

    int firstSpaceIndex = firstLine.find_first_of(' ');
    int lastSpaceIndex = firstLine.find_last_of(' ');
    int lastSlashIndex = firstLine.find_last_of('/');

    this->setMethod(firstLine.substr(0, firstSpaceIndex));
    this->setPath(firstLine.substr(firstSpaceIndex + 1, lastSpaceIndex - firstSpaceIndex - 1));
    this->setVersion(firstLine.substr(lastSlashIndex + 1));

    return this->deserializeHeadersAndContent(from);
}