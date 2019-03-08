#include "fileprovider.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int test_file_provider()
{
    FileProvider fileProvider("/tmp");

    ofstream testFile("/tmp/test.html");
    testFile << "Hello world!" << endl;
    testFile.flush();
    testFile.close();

    auto path = fileProvider.getFilePath("./../tmp/././test.html");
    if (path.compare("/tmp/test.html") != 0)
    {
        cout << "Expected /test.html to resolve to /tmp/test.html, got " << path << " instead." << endl;
        return 1;
    }

    path = fileProvider.getFilePath("../home/");
    if (path.compare("") != 0)
    {
        cout << "Expected ../home to resolve to 'non-existent', got " << path << " instead." << endl;
        return 1;
    }

    cout << "FileProvider works correctly" << endl;
    return 0;
}
