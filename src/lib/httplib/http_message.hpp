#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdbool.h>

using namespace std;

class HTTPMessage
{
public:
  HTTPMessage();

  string getVersion();
  void setVersion(string version);

  string getHeader(string name);
  string getHeader(string name, string defaultValue);
  void setHeader(string name, string value);

  void setContent(vector<uint8_t> content);
  void setContent(string content);
  vector<uint8_t> getContent();
  string getContentString();

protected:
  void serializeHeadersAndContent(ostream &into);
  bool deserializeHeadersAndContent(istream &from);
  void trim(string &value);

private:
  string version;
  map<string, string> headers;
  vector<uint8_t> content;

  bool deserializeHeaders(istream &from);
  bool deserializeContent(istream &from);
};