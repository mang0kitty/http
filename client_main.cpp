#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "httplib.h"
#include "sockstream.h"

using namespace std;

int http_request(string method, Url &url)
{
     cout << "Opening socket"
          << "\n";
     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
          cout << "Error opening socket: " << sock << "\n";
          return -1;
     }

     struct hostent *serverDnsRecord = gethostbyname(url.getHostname().c_str());
     if (serverDnsRecord == NULL)
     {
          cout << "Error locating server " << url.getHostname() << ": " << strerror(errno) << endl;
          return -1;
     }

     struct sockaddr_in serverAddress;

     bzero((char *)&serverAddress, sizeof(serverAddress));
     serverAddress.sin_family = AF_INET;
     bcopy((char *)serverDnsRecord->h_addr, (char *)&serverAddress.sin_addr.s_addr, serverDnsRecord->h_length);
     serverAddress.sin_port = htons(url.getPort());

     if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
     {
          printf("\nConnection Failed \n");
          return -1;
     }

     iosockstreambuf streamBuf(sock, 1024);
     iostream stream(&streamBuf);

     HTTPRequest request;
     request.setMethod(method);
     request.setPath(url.getPath());
     request.setHeader("Connection", "close");

     request.serialize(cout);
     request.serialize(stream);
     stream.flush();

     cout << endl
          << endl;

     HTTPResponse response;
     if (response.deserialize(stream))
     {
          if (response.getStatusCode() == 200)
          {
               // TODO: Create output directory if necessary
               ofstream outputFile("./" + url.getPath());
               outputFile << response.getContentString();
               outputFile.flush();
          }
          else
          {
               response.serialize(cout);
               cout << endl;
          }
     }
     else
     {
          cout << "Failed to get response from server" << endl;
     }

     shutdown(sock, SHUT_RDWR);

     return 0;
}

int client_main(vector<string> args)
{
     if (args.size() < 2)
     {
          cout << args[0] << " url..." << endl;
          cout << "  e.g. " << args[0] << " http://localhost:8001/index.html" << endl;
          return -1;
     }

     for (int i = 1; i < args.size(); i++)
     {
          Url url(args[i]);

          if (http_request("GET", url) != 0)
          {
               return -1;
          }
     }

     return 0;
}