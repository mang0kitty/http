#include "HTTP_Message.hpp"

HTTPMessage::HTTPMessage()
{
    this->setVersion("1.0");
    this->setHeader("Connection", "close");
    this->setHeader("User-Agent", "Aideen-HTTP/1.0");
}

void HTTPMessage::setVersion(string version)
{
    this->trim(version);
    this->version = version;
}

string HTTPMessage::getVersion()
{
    return this->version;
}

string HTTPMessage::getHeader(string name)
{
    return this->getHeader(name, "");
}

string HTTPMessage::getHeader(string name, string defaultValue)
{
    for (auto &&header : this->headers)
    {
        if (std::equal(
                name.begin(),
                name.end(),
                header.first.begin(),
                header.first.end(),
                [](char a, char b) {
                    return tolower(a) == tolower(b);
                }))
        {
            return header.second;
        }
    }

    return defaultValue;
}

void HTTPMessage::setHeader(string name, string value)
{
    this->trim(name);
    this->trim(value);
    this->headers[name] = value;
}

vector<uint8_t> HTTPMessage::getContent()
{
    return this->content;
}

string HTTPMessage::getContentString()
{
    return string(this->content.begin(), this->content.end());
}

void HTTPMessage::setContent(vector<uint8_t> content)
{
    this->content = content;

    // Business logic to ensure that our content length header matches our content
    this->setHeader("Content-Length", to_string(content.size()));
}

void HTTPMessage::setContent(string content)
{
    vector<uint8_t> contentVector(content.begin(), content.end());
    this->setContent(contentVector);
}

void HTTPMessage::serializeHeadersAndContent(ostream &into)
{
    for (auto &&header : this->headers)
    {
        into << header.first << ": " << header.second << "\r\n";
    }

    into << "\r\n";

    for (auto c : this->getContent())
    {
        into << (char)c;
    }
}

bool HTTPMessage::deserializeHeadersAndContent(istream &from)
{
    if (!this->deserializeHeaders(from))
    {
        return false;
    }

    if (this->getHeader("Content-Length").size() > 0)
    {
        if (!this->deserializeContent(from))
        {
            return false;
        }
    }

    return true;
}

bool HTTPMessage::deserializeHeaders(istream &from)
{
    string line;

    while (getline(from, line))
    {
        this->trim(line);

        if (line.size() == 0)
        {
            return true;
        }

        int splitIndex = line.find_first_of(':');

        string key = line.substr(0, splitIndex);
        string value = line.substr(splitIndex + 1);
        this->trim(key);
        this->trim(value);

        this->setHeader(key, value);
    }

    return true;
}

bool HTTPMessage::deserializeContent(istream &from)
{
    vector<uint8_t> data;
    data.resize(stoi(this->getHeader("Content-Length")));
    from.read((char *)&data[0], data.size());

    this->setContent(data);

    return true;
}

void HTTPMessage::trim(string &value)
{
    value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
    value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
}