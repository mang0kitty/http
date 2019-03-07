#include <sstream>
#include <iostream>
#include "httplib.h"
#include "testlib.h"

int test_http_response()
{
    HTTPResponse response(404, "Not Found");
    response.setContent("We couldn't find the page you were looking for :'(");
    response.setHeader("Content-Type", "text/plain");
    response.setHeader("Try-Again", "1d");

    stringstream responseStream("");
    response.serialize(responseStream);

    std::cout << "Our response is:\n\n"
              << responseStream.str()
              << "\n\n";

    HTTPResponse parsedResponse;
    parsedResponse.deserialize(responseStream);

    stringstream reserializedStream("");
    parsedResponse.serialize(reserializedStream);

    if (reserializedStream.str().compare(responseStream.str()) != 0)
    {
        Test::compareStrings(reserializedStream.str(), responseStream.str());
        exit(1);
    }
}