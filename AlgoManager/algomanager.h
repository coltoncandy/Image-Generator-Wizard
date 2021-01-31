#pragma once

#include <string>

namespace AlgoManager {
	class AlgoManager {
	public:
		static void grabCutWrapper(const std::string& path);
		static bool overlayWrapper(const std::string& bg, const std::string& fg); 
	};
}

