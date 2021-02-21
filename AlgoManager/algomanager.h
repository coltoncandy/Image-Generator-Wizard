#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <Windows.h>


namespace AlgoManager {
	class AlgoManager {
	public:
		static bool grabCutWrapper(const std::string& path);
		static void overlayWrapper(const std::string& bg, const std::string& fg); 
		static cv::Mat process(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath);
		static void batchProcess(const int imageNum, const std::string& initialPath, const std::string& targetPath,  std::string* backgroundPaths, const std::string& destinationPath, std::string*& imageBatch);
		static std::string createUniqueImageId(const std::string& destinationPath);
	};
}
