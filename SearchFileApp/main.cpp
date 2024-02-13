#include <iostream>
#include <string>
#include <filesystem>

#include "../SearchFileLibrary/ToSearchFile.hpp"

int main() {
	std::filesystem::path finalPath = SearchFile::ToSearchFile::SearchForFile("Adobe");
    std::cout << finalPath ? "\nFile was found!" : "\nFile not found!";
	return 0;
}