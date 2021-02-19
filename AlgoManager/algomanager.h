#pragma once
#include <string>
#include <opencv2/core.hpp>


namespace AlgoManager {
	class AlgoManager {
	public:
		static bool grabCutWrapper(const std::string& path);
		static void overlayWrapper(const std::string& bg, const std::string& fg); 
		static cv::Mat processor(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, const std::string& destinationPath);
	};
}
