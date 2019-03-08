#pragma once
#include <string>

class FileProvider
{
  public:
    FileProvider(std::string root);
    std::string getFilePath(std::string path);

  private:
    std::string root;
};