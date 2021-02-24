#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <vector>

namespace AlgoManager {
	class AlgoManager {
	public:
		static bool grabCutWrapper(const std::string& path);
		static void overlayWrapper(const std::string& bg, const std::string& fg); 
		static cv::Mat process(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, int randomNumber);
		static void batchProcess(int imageNum, const std::string& initialPath, const std::string& targetPath,  std::string* backgroundPaths, std::vector<cv::Mat>& imageBatch);
	};
}
