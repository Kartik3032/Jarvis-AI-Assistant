#include "../include/file_manager.h"

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

void FileManager::CreateFolder(const std::string &folderName)
{
    if (fs::create_directory(folderName))
    {
        std::cout << "Folder Created Successfully\n";
    }
    else
    {
        std::cout << "Folder already exists or cannot be created\n";
    }
}
void FileManager::DeleteFolder(const std::string &folderName)
{
    if (std::filesystem::remove(folderName))
    {
        std::cout << "Folder Deleted Successfully\n";
    }
    else
    {
        std::cout << "Folder Not Found\n";
    }
}
void FileManager::CreateFile(const std::string &fileName)
{
    std::ofstream file(fileName);

    if (file)
    {
        std::cout << "File Created Successfully\n";
    }
    else
    {
        std::cout << "Unable to Create File\n";
    }
}

void FileManager::DeleteFile(const std::string &fileName)
{
    if (std::filesystem::remove(fileName))
    {
        std::cout << "File Deleted Successfully\n";
    }
    else
    {
        std::cout << "File Not Found\n";
    }
}
void FileManager::Rename(const std::string &oldName,
                         const std::string &newName)
{
    std::filesystem::rename(oldName, newName);

    std::cout << "Renamed Successfully\n";
}
void FileManager::ListFiles()
{
    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        std::cout << entry.path().filename().string() << '\n';
    }
}