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

int client_main(vector<string> args)
{
     if (args.size() < 4)
     {
          cout << args[0] << " " << args[1] << " method path" << endl;
          cout << "  e.g. " << args[0] << " " << args[1] << " GET /index.html" << endl;
          return -1;
     }

     // TODO: Don't hard-code the server host and port
     string host = "localhost";
     int port = 8001;
     string method = args[2];
     string path = args[3];

     cout << "Opening socket"
          << "\n";
     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
          cout << "Error opening socket: " << sock << "\n";
          return -1;
     }

     struct hostent *serverDnsRecord = gethostbyname(host.c_str());
     if (serverDnsRecord == NULL)
     {
          cout << "Error locating server " << host << ": " << strerror(errno) << endl;
          return -1;
     }

     struct sockaddr_in serverAddress;

     bzero((char *)&serverAddress, sizeof(serverAddress));
     serverAddress.sin_family = AF_INET;
     bcopy((char *)serverDnsRecord->h_addr, (char *)&serverAddress.sin_addr.s_addr, serverDnsRecord->h_length);
     serverAddress.sin_port = htons(port);

     //connect to server
     if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
     {
          printf("\nConnection Failed \n");
          return -1;
     }

     iosockstreambuf streamBuf(sock, 1024);
     iostream stream(&streamBuf);

     HTTPRequest request;
     request.setMethod(method);
     request.setPath(path);
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
               ofstream outputFile("./" + path);
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