#include "httplib.h"
#include <stdio.h>

using namespace std;

int test_url()
{
    Url test1("http://www.google.com/search");
    if (test1.getProtocol().compare("http") != 0)
    {
        cout << "Expected test1 protocol to be http, got: " << test1.getProtocol() << endl;
        return 1;
    }

    if (test1.getHostname().compare("www.google.com") != 0)
    {
        cout << "Expected test1 host to be www.google.com, got: " << test1.getHostname() << endl;
        return 1;
    }

    if (test1.getPort() != 80)
    {
        cout << "Expected test1 port to be 80, got: " << test1.getPort() << endl;
        return 1;
    }

    if (test1.getPath().compare("/search") != 0)
    {
        cout << "Expected test1 path to be /search, got: " << test1.getPath() << endl;
        return 1;
    }

    cout << "Passed URL test1" << endl;

    Url test2("localhost:8001/search?q=test");
    if (test2.getProtocol().compare("http") != 0)
    {
        cout << "Expected test1 protocol to be http, got: " << test2.getProtocol() << endl;
        return 1;
    }

    if (test2.getHostname().compare("localhost") != 0)
    {
        cout << "Expected test1 host to be localhost, got: " << test2.getHostname() << endl;
        return 1;
    }

    if (test2.getPort() != 8001)
    {
        cout << "Expected test2 port to be 8001, got: " << test2.getPort() << endl;
        return 1;
    }

    if (test2.getPath().compare("/search?q=test") != 0)
    {
        cout << "Expected test2 path to be /search?q=test, got: " << test2.getPath() << endl;
        return 1;
    }

    cout << "Passed URL test2" << endl;

    return 0;
}
