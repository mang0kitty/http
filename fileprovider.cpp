#include "fileprovider.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>

FileProvider::FileProvider(std::string root)
{
    this->root = root;
}

std::string FileProvider::getFilePath(std::string path)
{
    auto fullPath = root + "/" + path;
    auto truePath = realpath(fullPath.c_str(), NULL);
    if (truePath == NULL)
    {
        return "";
    }

    std::string actualPath(truePath);
    free(truePath);

    if (actualPath.size() < root.size())
        return "";
    if (std::equal(root.begin(), root.end(), actualPath.begin()))
        return actualPath;

    return "";
}