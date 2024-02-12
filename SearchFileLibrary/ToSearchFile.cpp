#include "ToSearchFile.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace SearchFile {
	std::string ToSearchFile::SearchForFile(std::string fileName) {
		std::string filePath = "nice";
        bool fileFound = false;

        std::string rootDit = "C:\\";

        for (const auto& subDir : fs::directory_iterator(rootDit)) {
            if (subDir.path().filename() == fileName) {
                std::cout << subDir.path() << std::endl;
                fileFound = true;
            }
        }

        if (fileFound) std::cout << "Succesfully found file: '" << fileName << "'!\n";
        else std::cout << "File: '" << fileName << "' was not found!";

		return filePath;
	}
}