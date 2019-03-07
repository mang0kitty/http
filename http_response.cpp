#include "HTTP_Response.hpp"

HTTPResponse::HTTPResponse() : HTTPMessage()
{
    this->setStatusCode(200);
    this->setReasonPhrase("OK");
}

HTTPResponse::HTTPResponse(int statusCode, string reasonPhrase) : HTTPMessage()
{
    this->setStatusCode(statusCode);
    this->setReasonPhrase(reasonPhrase);
}

HTTPResponse::HTTPResponse(int statusCode, string reasonPhrase, vector<uint8_t> content) : HTTPMessage()
{
    this->setStatusCode(statusCode);
    this->setReasonPhrase(reasonPhrase);
    this->setContent(content);
}

void HTTPResponse::setStatusCode(int statusCode)
{
    this->statusCode = statusCode;
}
int HTTPResponse::getStatusCode()
{
    return this->statusCode;
}

void HTTPResponse::setReasonPhrase(string reasonPhrase)
{
    this->trim(reasonPhrase);
    this->reasonPhrase = reasonPhrase;
}
string HTTPResponse::getReasonPhrase()
{
    return this->reasonPhrase;
}

void HTTPResponse::serialize(ostream &into)
{
    into << "HTTP/" << this->getVersion() << " " << this->getStatusCode() << " " << this->getReasonPhrase() << "\r\n";
    this->serializeHeadersAndContent(into);
}

bool HTTPResponse::deserialize(istream &from)
{
    string firstLine;
    if (!getline(from, firstLine))
    {
        return false;
    }

    int slashIndex = firstLine.find_first_of('/');
    int firstSpaceIndex = firstLine.find_first_of(' ');
    int secondSpaceIndex = firstLine.find_first_of(' ', firstSpaceIndex + 1);

    this->setVersion(firstLine.substr(slashIndex + 1, firstSpaceIndex - slashIndex - 1));
    this->setStatusCode(stoi(firstLine.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1)));
    this->setReasonPhrase(firstLine.substr(secondSpaceIndex + 1));

    return this->deserializeHeadersAndContent(from);
}