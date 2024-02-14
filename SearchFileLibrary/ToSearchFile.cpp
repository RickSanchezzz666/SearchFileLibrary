#include "ToSearchFile.hpp"

#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace fs = std::filesystem;

std::mutex directoriesMutex;
std::mutex filePathMutex;

namespace SearchFile {
    fs::path ToSearchFile::__checkStatus(const std::string& fileName, bool& status, const fs::directory_entry& directory) {
        const std::string name = directory.path().filename().string();
        if (name == fileName) {
            std::lock_guard<std::mutex> lock(filePathMutex);
            status = true;
            return directory.path();
        }
        return fs::path(); 
    }

    fs::path ToSearchFile::__checkInitialDirectory(const std::string& fileName, bool& status, const std::string& directory, std::vector<std::string>& directories) {
        for (const auto& subDir : fs::directory_iterator(directory)) {
            fs::file_status fileStatus = fs::status(subDir);
            if (fs::is_directory(fileStatus)) {
                fs::path path = __checkStatus(fileName, status, subDir);
                if (path != fs::path() && status) return path;
                std::lock_guard<std::mutex> lock(directoriesMutex);
                directories.push_back(subDir.path().string());
            }
            else if (fs::is_regular_file(fileStatus)) {
                fs::path path = __checkStatus(fileName, status, subDir);
                if (path != fs::path() && status) return path;
            }
        }
        return fs::path();
    }


    void ToSearchFile::__createThread(std::vector<std::string>& directories, fs::path& filePath, const std::string& fileName, bool& status) {
        std::vector<std::thread> threads;
        std::vector<std::vector<std::string>> filteredDirectories(__maxThreadNumber);
        int it = 0;
        for (const auto& directory : directories) {
            if (it < __maxThreadNumber) {
                filteredDirectories[it].push_back(directory);
                it++;
            }
            else it = 0;
        }

        for (const std::vector<std::string>& directories : filteredDirectories) {
            auto threadFunc = [&]() { return __checkSubDirectory(fileName, filePath, status, directories); };
            threads.emplace_back(threadFunc);
            std::cout << "\nThread created!";
        }
        std::cout << std::endl;

        for (auto& thread : threads) {
            std::cout << thread.joinable() << std::endl;
            thread.join();
            std::cout << "Thread joined!\n";
        }

        return;
    }

   /* void ToSearchFile::__startThread(const std::string& fileName, fs::path& filePath, bool& status, const std::vector<std::string>& directories) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.1s);
        for (const auto& directory : directories) {
            __checkSubDirectory(fileName, filePath, status, directory);
            if (status) return;
        }
        std::this_thread::sleep_for(0.2s);
    }*/

    void ToSearchFile::__checkSubDirectory(const std::string& fileName, fs::path& filePath, bool& status, const std::vector<std::string>& directories) {
        std::vector<std::string> subDirectories;
        for (const auto& directory : directories) {
            for (auto& subDir : fs::directory_iterator(directory)) {
                if (status) return;
                fs::file_status fileStatus = fs::status(subDir);
                if (fs::is_directory(fileStatus)) {
                    fs::path path = __checkStatus(fileName, status, subDir);
                    if (path != fs::path() && status) {
                        std::lock_guard<std::mutex> lock(filePathMutex);
                        filePath = path;
                        return;
                    };
                    subDirectories.push_back(subDir.path().string());
                }
                else if (fs::is_regular_file(fileStatus)) {
                    fs::path path = __checkStatus(fileName, status, subDir);
                    if (path != fs::path() && status) {
                        std::lock_guard<std::mutex> lock(filePathMutex);
                        filePath = path;
                        return;
                    };
                }
                //if ((fileStatus.permissions() & fs::perms::owner_read) != fs::perms::none)
            }
            return;
        }
        if (!subDirectories.empty()) {
            __checkSubDirectory(fileName, filePath, status, subDirectories);
        }
        else return;
    }

    fs::path ToSearchFile::SearchForFile(std::string fileName) {
        try
        { 
            std::vector<std::string> directories;
            bool foundStatus = false;

            std::string rootDir = "C:\\";
            fs::path filePath = __checkInitialDirectory(fileName, foundStatus, rootDir, directories);

            if (!foundStatus) __createThread(directories, filePath, fileName, foundStatus);

            if (foundStatus) std::cout << "\nSuccesfully found file: '" << fileName << "'!\n" << "Path to this file " << filePath << "!\n";
            else std::cout << "\nFile: '" << fileName << "' was not found!";

            return filePath;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
	}
        
    
}