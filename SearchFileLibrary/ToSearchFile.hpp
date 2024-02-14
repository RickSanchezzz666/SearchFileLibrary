#pragma once
#include <string>

#include <filesystem>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;

namespace SearchFile {
	class ToSearchFile {
    private:
        static const int __maxThreadNumber = 4;

        static fs::path __checkStatus(const std::string& fileName, bool& status, const fs::directory_entry& directory);
        
        static fs::path __checkInitialDirectory(const std::string& fileName, bool& status, const std::string& directory, std::vector<std::string>& directories);

        static bool __checkIfFolderIsAvailable(const fs::directory_entry& directory);

        static void __createThread(std::vector<std::string>& directories, fs::path& filePath, const std::string& fileName, bool& status);
        
        static fs::path __startSearching(std::vector<std::string>& directories, fs::path& filePath, const std::string& fileName, bool& foundStatus);

        static void __checkDeepDirectory(const std::string& fileName, fs::path& filePath, bool& status, const std::string& directories);
        static void __checkSubDirectory(const std::string& fileName, fs::path& filePath, bool& status, const std::vector<std::string>& directories);
	public:
        static fs::path SearchForFile(std::string fileName);
	};
}