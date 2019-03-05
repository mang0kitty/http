#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "httplib.h"
#include "sockstream.h"

using namespace std;

int client_main()
{
     cout << "Opening socket"
          << "\n";
     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
          cout << "Error opening socket: " << sock << "\n";
          return -1;
     }

     struct sockaddr_in serverAddress;
     bzero((char *)&serverAddress, sizeof(serverAddress));
     serverAddress.sin_family = AF_INET;
     serverAddress.sin_addr.s_addr = INADDR_ANY;
     serverAddress.sin_port = htons(8001);

     cout << "Binding socket"
          << "\n";
     if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
     {
          cout << "Error binding socket"
               << "\n";
          return -1;
     }

     cout << "Listening on socket"
          << "\n";
     listen(sock, 10);

     while (true)
     {
          struct sockaddr_in remoteHost;
          socklen_t remoteHostLen;

          cout << "Waiting for connection"
               << "\n";
          int conn = accept(sock, (struct sockaddr *)&remoteHost, &remoteHostLen);

          if (conn < 0)
          {
               cout << "Error accepting new connection: " << conn << "\n";
               continue;
          }

          cout << "Accepted new connection"
               << "\n";

          iosockstreambuf sockBuf(conn, 1024);
          iostream sockStream(&sockBuf);

          HTTPRequest request;
          request.deserialize(sockStream);

          request.serialize(cout);

          cout << "\n\n";

          HTTPResponse response(200, "OK");
          response.setHeader("Connection", "close");
          response.setContent("Thanks for your request!");

          response.serialize(sockStream);
          response.serialize(cout);
          cout << "\n\n";
          sockStream.flush();

          shutdown(conn, SHUT_RDWR);
     }
     //testRequest();
     //testResponse();

     return 0;
}