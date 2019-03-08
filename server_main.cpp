#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <list>
#include "client.hpp"
#include "fileprovider.hpp"

using namespace std;

int startListeningServer(struct sockaddr *serverAddress, socklen_t serverAddressLen)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cout << "Error opening socket: " << strerror(errno) << "\n";
        return -1;
    }

    if (bind(sock, serverAddress, serverAddressLen) < 0)
    {
        cout << "Error binding socket: " << strerror(errno)
             << "\n";
        return -1;
    }

    if (listen(sock, 10) < 0)
    {
        cout << "Error listening on socket: " << strerror(errno) << endl;
        return -1;
    }

    return sock;
}

int acceptConnection(int serverSocket)
{
    struct sockaddr_in remoteHost;
    socklen_t remoteHostLen;

    int conn = accept(serverSocket, (struct sockaddr *)&remoteHost, &remoteHostLen);

    if (conn < 0)
    {
        cout << "Error accepting new connection: " << strerror(errno) << "(" << conn << ")"
                                                                                        "\n";
        return -1;
    }

    cout << "Accepted new connection"
         << "\n";
    return conn;
}

int server_main(vector<string> args)
{
    if (args.size() < 4)
    {
        cout << args[0] << " hostname port file-dir" << endl;
        return -1;
    }

    auto hostname = args[1];
    int port = stoi(args[2]);
    auto directory = args[3];

    FileProvider fileProvider(directory);

    struct hostent *serverDnsRecord = gethostbyname(hostname.c_str());
    if (serverDnsRecord == NULL)
    {
        cout << "Error locating server " << hostname << ": " << strerror(errno) << endl;
        return -1;
    }

    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)serverDnsRecord->h_addr, (char *)&serverAddress.sin_addr.s_addr, serverDnsRecord->h_length);
    serverAddress.sin_port = htons(port);

    int sock = startListeningServer((struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (sock < 0)
    {
        std::cout << "Failed to start server, exiting..." << std::endl;
        return 1;
    }

    cout << "Listening on " << hostname << ":" << port << " in " << directory
         << "\n";

    std::list<Client *> clients;

    while (true)
    {
        int conn = acceptConnection(sock);
        if (conn < 0)
        {
            continue;
        }

        auto client = new Client(conn, &fileProvider);
        client->start();

        //clients.push_back(client);
        // TODO: Shutdown the client if it is running for >20s with no traffic
    }

    return 0;
}