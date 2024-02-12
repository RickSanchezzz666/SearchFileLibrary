#pragma once
#include <string>

#include <filesystem>

namespace SearchFile {
	class ToSearchFile {
	public:
		static std::string SearchForFile(std::string fileName);
	};
}