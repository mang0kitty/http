#include <iostream>
#include <string>
#include <vector>
#include "apps.h"

void parseArgs(int argc, const char *argv[], std::vector<std::string> &target)
{
    for (int i = 0; i < argc; i++)
    {
        target.push_back(std::string(argv[i]));
    }
}

void showHelp(std::vector<std::string> args)
{
    std::cout << args[0] << " [mode] [args...]" << std::endl;
    std::cout << std::endl;
    std::cout << "By Aideen Fay" << std::endl;
    std::cout << std::endl;
    std::cout << " Modes:" << std::endl;
    std::cout << "   - server [directory]" << std::endl;
    std::cout << "     e.g. server ./files" << std::endl;
    std::cout << std::endl;
    std::cout << "   - client [method] [url]" << std::endl;
    std::cout << "     e.g. client GET http://localhost:8000/index.html" << std::endl;
    std::cout << std::endl;
    std::cout << "   - test [suite]" << std::endl;
    std::cout << "     e.g. test http_request" << std::endl;
    std::cout << "     e.g. test http_response" << std::endl;
}

int main(int argc, const char *argv[])
{
    std::vector<std::string> args;
    parseArgs(argc, argv, args);

    if (args.size() < 2)
    {
        showHelp(args);
        return 1;
    }

    std::string mode = args[1];

    if (mode.compare("test") == 0)
    {
        int code = 0;
        code += test_http_request();
        code += test_http_response();
        return code;
    }
    else if (mode.compare("server") == 0)
    {
        return server_main();
    }
    else if (mode.compare("client") == 0)
    {
        return client_main();
    }
    else
    {
        showHelp(args);
        return 1;
    }
}