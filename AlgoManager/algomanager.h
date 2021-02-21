#pragma once
#include <string>
#include <Windows.h>

namespace AlgoManager {
	class AlgoManager {
	public:
		static bool grabCutWrapper(const std::string& path);
		static void overlayWrapper(const std::string& bg, const std::string& fg); 
		static std::string process(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, const std::string& destinationPath);
		static void batchProcess(const int imageNum, const std::string& initialPath, const std::string& targetPath,  std::string* backgroundPaths, const std::string& destinationPath, std::string*& imageBatch);
		static std::string createUniqueImageId(const std::string& destinationPath);
	};
}
