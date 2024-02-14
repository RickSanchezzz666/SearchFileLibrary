#include "ToSearchFile.hpp"

#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace fs = std::filesystem;

std::mutex threadMutex;
std::mutex filePathMutex;
std::mutex statusMutex;

std::vector<std::string> forbiddenFiles;


namespace SearchFile {
    fs::path ToSearchFile::__checkStatus(const std::string& fileName, bool& status, const fs::directory_entry& directory) {
        const std::string name = directory.path().filename().string();
        if (name == fileName) {
            std::lock_guard<std::mutex> lock(statusMutex);
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
                if (__checkIfFolderIsAvailable(subDir)) directories.push_back(subDir.path().string());
            }
            else if (fs::is_regular_file(fileStatus)) {
                fs::path path = __checkStatus(fileName, status, subDir);
                if (path != fs::path() && status) return path;
            }
        }
        return fs::path();
    }

    bool ToSearchFile::__checkIfFolderIsAvailable(const fs::directory_entry& directory) {
        for (auto file : forbiddenFiles) {
            if (directory.path().string() == file) return false;
        }
        return true;
    }


    /*void ToSearchFile::__createThread(std::vector<std::string>& directories, fs::path& filePath, const std::string& fileName, bool& status) {
        std::vector<std::vector<std::string>> filteredDirectories(__maxThreadNumber);
        std::vector<std::thread> threads;
        int it = 0;
        for (const auto& directory : directories) {
            if (it < __maxThreadNumber) {
                filteredDirectories[it].push_back(directory);
                it++;
            }
            else it = 0;
        }

        for (const std::vector<std::string>& directories : filteredDirectories) {
            auto threadFunc = [&directories, &filePath, &fileName, &status]() {
                __checkSubDirectory(fileName, filePath, status, directories);
                };
            threads.emplace_back(threadFunc);
            std::cout << "Thread created!\n";
        }
        std::cout << std::endl;

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);

        for (auto& thread : threads) {
            thread.join();
        }

        
        std::thread::id id = std::this_thread::get_id();
        for (int i = 0; i < __maxThreadNumber; i++) {
            if (threads[i].get_id() == id) {
                threads[i].join();
                std::cout << "Thread joined!\n";
                threads.erase(threads.begin() + i);
            }
        }

        return;
    }*/

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
                    if (__checkIfFolderIsAvailable(subDir)) __checkDeepDirectory(fileName, filePath, status, subDir.path().string());
                }
                else if (fs::is_regular_file(fileStatus)) {
                    fs::path path = __checkStatus(fileName, status, subDir);
                    if (path != fs::path() && status) {
                        std::lock_guard<std::mutex> lock(filePathMutex);
                        filePath = path;
                        return;
                    };
                }
            }
        }
    }

    void ToSearchFile::__checkDeepDirectory(const std::string& fileName, fs::path& filePath, bool& status, const std::string& directories) {
        for (auto& subDir : fs::directory_iterator(directories)) {
            if (status) return;
            fs::file_status fileStatus = fs::status(subDir);
            if (fs::is_directory(fileStatus)) {
                fs::path path = __checkStatus(fileName, status, subDir);
                if (path != fs::path() && status) {
                    std::lock_guard<std::mutex> lock(filePathMutex);
                    filePath = path;
                    return;
                };
                if (__checkIfFolderIsAvailable(subDir)) __checkDeepDirectory(fileName, filePath, status, subDir.path().string());
            }
            else if (fs::is_regular_file(fileStatus)) {
                fs::path path = __checkStatus(fileName, status, subDir);
                if (path != fs::path() && status) {
                    std::lock_guard<std::mutex> lock(filePathMutex);
                    filePath = path;
                    return;
                };
            }
        }
    }

    fs::path ToSearchFile::SearchForFile(std::string fileName) {
        try
        { 
            std::vector<std::string> directories;
            bool foundStatus = false;

            std::string rootDir = "C:\\";
            fs::path filePath = __checkInitialDirectory(fileName, foundStatus, rootDir, directories);

            //if (!foundStatus) __createThread(directories, filePath, fileName, foundStatus);
            if (!foundStatus) {
                __checkSubDirectory(fileName, filePath, foundStatus, directories);
            }

            if (foundStatus) std::cout << "\nSuccesfully found file: '" << fileName << "'!\n" << "Path to this file " << filePath << "!\n";
            else std::cout << "\nFile: '" << fileName << "' was not found!";

            return filePath;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::string result;
            std::string str = e.what();
            size_t startPos = str.find("\"");
            if (startPos != std::string::npos) {
                size_t endPos = str.find("\"", startPos + 1);
                if (endPos != std::string::npos) {
                    result = str.substr(startPos + 1, endPos - startPos - 1);
                }
            }
            forbiddenFiles.push_back(result);
            SearchForFile(fileName);
        }
	}
        
    
}