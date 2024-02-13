#pragma once
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

namespace SearchFile {
	class ToSearchFile {
    private:
        static fs::path __setStatus(const std::string& fileName, bool& status, const fs::directory_entry& directory);

        static fs::path __checkSubDirectory(const std::string& fileName, bool& status, const std::string& directory);
        static fs::path __checkSubDirectory(const std::string& fileName, bool& status, const fs::directory_entry& directory);
	public:
        static fs::path SearchForFile(std::string fileName);
	};
}