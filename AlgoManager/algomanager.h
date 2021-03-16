#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <vector>

namespace AlgoManager {
	class AlgoManager {
	public:
		static bool grabCutWrapper(const std::string& path);
		static cv::Mat process(const std::string& targetPath, const std::string& backgroundPath);
		static cv::Mat processTarget(cv::Mat target);
		static cv::Mat processBackground(cv::Mat &background, const cv::Mat target);
		static void batchProcess(int imageNum, const std::string& targetPath,  std::string* backgroundPaths, std::vector<cv::Mat>& imageBatch);
	};
}
