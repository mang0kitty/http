#include <iostream>
#include <string>
#include <vector>

int server_main(std::vector<std::string> args);
int client_main(std::vector<std::string> args);
int test_http_request();
int test_http_response();
int test_url();

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

    std::string mode = args[0];

    if (mode.find("web-test") != std::string::npos)
    {
        // TODO: This should be replaced with a test_main(args) method
        int code = 0;
        code += test_http_request();
        code += test_http_response();
        code += test_url();
        return code;
    }
    else if (mode.find("web-server") != std::string::npos)
    {
        // TODO: We should pass args into this method
        return server_main(args);
    }
    else if (mode.find("web-client") != std::string::npos)
    {
        return client_main(args);
    }
    else
    {
        showHelp(args);
        return 1;
    }
}