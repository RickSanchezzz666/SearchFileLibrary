#include <iostream>
#include <string>
#include <filesystem>

#include "../SearchFileLibrary/ToSearchFile.hpp"

int main() {
	std::filesystem::path finalPath = SearchFile::ToSearchFile::SearchForFile("Program Files");
	std::cin.get();
	return 0;
}