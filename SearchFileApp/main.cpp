#include <iostream>
#include <string>
#include <filesystem>

#include "../SearchFileLibrary/ToSearchFile.hpp"

int main() {
	std::filesystem::path finalPath = SearchFile::ToSearchFile::SearchForFile("Microsoft.AnalysisServices.AzureClient.dll");
	return 0;
}