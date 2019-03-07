#pragma once
#include "HTTP_Message.hpp"

class HTTPResponse : public HTTPMessage
{
  private:
    string reasonPhrase;
    int statusCode;

  public:
    HTTPResponse();
    HTTPResponse(int statusCode, string reasonPhrase);
    HTTPResponse(int statusCode, string reasonPhrase, vector<uint8_t> content);

    void setStatusCode(int statusCode);
    int getStatusCode();

    void setReasonPhrase(string reasonPhrase);
    string getReasonPhrase();

    void serialize(ostream &into);
    bool deserialize(istream &from);
};