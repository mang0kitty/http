#pragma once
#include "HTTP_Message.hpp"

class HTTPRequest : public HTTPMessage
{
public:
  HTTPRequest();
  HTTPRequest(string method, string path);
  HTTPRequest(string method, string path, vector<uint8_t> content);

  void setMethod(string method); //in function call table
  string getMethod();
  void setPath(string path);
  string getPath();

  void serialize(ostream &into);
  bool deserialize(istream &from);

private:
  string method;
  string path;
};