#pragma once

#include <string>

class FileManager
{
public:
    static void CreateFolder(const std::string &folderName);
    static void DeleteFolder(const std::string &folderName);
    static void CreateFile(const std::string &fileName);
    static void DeleteFile(const std::string &fileName);
};