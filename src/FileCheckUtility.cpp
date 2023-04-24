#include "FileCheckUtility.h"
#include <iostream>
#include <string>
#include <dirent.h>
#include <streambuf>
#include <sstream>

std::string CheckFilePath(const DataHeaderStruct & header)
{
    std::string filePath = LOCATION + header.userName;
    if ( !boost::filesystem::exists( filePath ) )
    {
        boost::filesystem::create_directory(filePath);
        filePath += "/" + std::to_string(header.id);
        boost::filesystem::create_directory(filePath);
    }
    else
    {
        filePath += "/" + std::to_string(header.id);
        if ( !boost::filesystem::exists( filePath ) )
        {
            boost::filesystem::create_directory(filePath);
        }
    }
    return filePath + "/";
}

std::string GetFile(dirent* entry, std::string base)
{
    dirent* ent;
    std::string folderName(entry->d_name);
    DIR* directory = opendir((base + "/" + folderName).c_str());
    while((ent = readdir(directory))  != nullptr)
    {
        std::string filename = ent->d_name;
        if (filename != "." && filename != "..") 
        {
            return filename;
        }
    }
    closedir(directory);
    return "";
}

std::string GetListOfUserFiles(const std::string & userName)
{
    std::stringstream userFiles;
    std::string directory_path = LOCATION + userName;
    DIR* directory = opendir(directory_path.c_str());
    if (directory == nullptr) 
    {
        std::cerr << "Error: could not open directory" << std::endl;
        return "";
    }
    dirent* entry;
    while ((entry = readdir(directory)) != nullptr) 
    {
        userFiles << std::string(entry->d_name) << ":";
        std::cout << GetFile(entry, directory_path) << std::endl;
        userFiles << GetFile(entry, directory_path) << std::endl;     
    }
    closedir(directory);

    return userFiles.str();
}

/*std::string GetListOfUserFiles(const std::string & userName)
{
    std::stringstream userFiles;
    boost::filesystem::path path(LOCATION + userName);
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr)
    {
 //       if (boost::filesystem::is_regular_file(itr->path())) 
        {
            // assign current file name to current_file and echo it out to the console.

            if(boost::filesystem::is_directory(itr->path()))
            {
        
                std::string current_folder = itr->path().string();
                std::cout << current_folder << std::endl;
                userFiles << current_folder << " : ";
                boost::filesystem::directory_iterator dir_itr;
                for (boost::filesystem::directory_iterator dir(itr->path()); dir != dir_itr; ++dir)
                {
                    if (boost::filesystem::is_regular_file(dir_itr->path())) 
                    {
                        std::string file = dir_itr->path().filename().string();
                        std::cout << file << std::endl;
                        userFiles << file << std::endl;
                    }
                }
            }

        }
    }

    return userFiles.str();
}*/