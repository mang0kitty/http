#include <sstream>
#include <iostream>
#include "httplib.h"
#include "testlib.h"

int test_http_request()
{
    HTTPRequest request("GET", "/index.html");

    request.setHeader("Accept", "text/html");
    request.setContent("Test content!");

    stringstream requestStream("");
    request.serialize(requestStream);

    std::cout << "Our request is:\n\n"
              << requestStream.str()
              << "\n\n";

    HTTPRequest parsedRequest;
    parsedRequest.deserialize(requestStream);

    stringstream reserializedStream("");
    parsedRequest.serialize(reserializedStream);

    if (reserializedStream.str().compare(requestStream.str()) != 0)
    {
        Test::compareStrings(reserializedStream.str(), requestStream.str());
        exit(1);
    }
}