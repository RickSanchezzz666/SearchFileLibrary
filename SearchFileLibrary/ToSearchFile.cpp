#include "ToSearchFile.hpp"

#include <iostream>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

namespace SearchFile {
    fs::path ToSearchFile::__setStatus(const std::string& fileName, bool& status, const fs::directory_entry& directory) {
        if (directory.path().filename() == fileName) {
            status = true;
            return directory.path();
        }
        else return fs::path();
    }
    fs::path ToSearchFile::__checkSubDirectory(const std::string& fileName, bool& status, const std::string& directory) {
        for (const auto& subDir : fs::directory_iterator(directory)) {
            try {
                std::string name = subDir.path().filename().string();

                if (name[0] == '$') continue;
                fs::file_status fileStatus = fs::status(subDir);
                fs::path path = __setStatus(fileName, status, subDir);
                if (fs::exists(fileStatus)) {
                    if (fs::is_regular_file(fileStatus))
                        path = __setStatus(fileName, status, subDir);
                    else if (fs::is_directory(fileStatus)) {
                        if ((fileStatus.permissions() & fs::perms::owner_read) != fs::perms::none) {
                            path = __checkSubDirectory(fileName, status, subDir);
                        }
                    }
                }
                else path = __setStatus(fileName, status, subDir);
                if (path != fs::path()) return path;
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }

    fs::path ToSearchFile::__checkSubDirectory(const std::string& fileName, bool& status, const fs::directory_entry& directory) {
        for (const auto& subDir : fs::directory_iterator(directory)) {
            try {
                fs::file_status fileStatus = fs::status(subDir);
                fs::path path = __setStatus(fileName, status, subDir);
                if (fs::exists(fileStatus)) {
                    if (fs::is_regular_file(fileStatus))
                        path = __setStatus(fileName, status, subDir);
                    else if (fs::is_directory(fileStatus)) {
                        if ((fileStatus.permissions() & fs::perms::owner_read) != fs::perms::none) {
                            path = __checkSubDirectory(fileName, status, subDir);
                        }
                    } 
                }
                else path = __setStatus(fileName, status, subDir);
                if (path != fs::path()) return path;
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }

    fs::path ToSearchFile::SearchForFile(std::string fileName) {
        try
        {
            bool foundStatus = false;
            short threadsNumber = 0;

            std::string rootDir = "C:\\";
            fs::path filePath = filePath = __checkSubDirectory(fileName, foundStatus, rootDir);;

            if (foundStatus) std::cout << "Succesfully found file: '" << fileName << "'!\n" << "Path to this file " << filePath << "!\n";
            else std::cout << "File: '" << fileName << "' was not found!";

            return filePath;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
	}
        
    
}