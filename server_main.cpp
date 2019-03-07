#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "client.hpp"

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
        cout << "Error accepting new connection: " << strerror(errno) << "\n";
        return -1;
    }

    cout << "Accepted new connection"
         << "\n";
    return conn;
}

int server_main(vector<string> args)
{
    // TODO: Accept command line arguments for hostname, port and directory
    // TODO: Pass directory info to clients (to locate files)

    char cwdBuf[1024];
    getcwd(cwdBuf, sizeof(cwdBuf));
    // if (chroot(cwdBuf) != 0)
    // {
    //     std::cout << "Failed to chroot the web server: " << strerror(errno) << endl;
    //     return 1;
    // }

    string directory = string(cwdBuf);
    int port = 8001;

    cout << "Running in " << directory << endl;

    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int sock = startListeningServer((struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (sock < 0)
    {
        std::cout << "Failed to start server, exiting..." << std::endl;
        return 1;
    }

    cout << "Listening on 0.0.0.0:" << port
         << "\n";

    std::vector<Client *> clients;

    while (true)
    {
        int conn = acceptConnection(sock);
        if (conn < 0)
        {
            continue;
        }

        auto client = new Client(conn);
        client->start();
        //clients.push_back(client);
    }

    return 0;
}