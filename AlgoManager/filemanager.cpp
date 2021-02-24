#include "fileManager.h"

//Randomly fills the array of strings, imageList, with paths of images found in the directory
//specified by the absolutePath. Unique images will always be added first. If there are more 
//images requested than existing in the directory provided, repeat images will be added.
//The number of images added to imageList is set by the imageNum parameter.
void getRandomImages(int imageNum, std::string absolutePath, std::string *& imageList) {
	if(imageNum < 1) {
		std::string errorMessage = "The number of background selected to return must be greater than 0.";
		throw errorMessage;
	}

	if(imageList) {
		delete[] imageList;
		imageList = nullptr;
	}

	absolutePath.append("/");
	std::string searchPath = absolutePath;
	searchPath.append("*");
    WIN32_FIND_DATAA data;

    HANDLE hFind = FindFirstFileA(searchPath.c_str() , &data);
    if(hFind == INVALID_HANDLE_VALUE) {
		std::string errorMessage = "Could not find given directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
    }

	std::vector<std::string> fileList;

	//Retrieves all files from directory and add thir paths to the fileList
	do {
		std::string file = data.cFileName;
		//Only adds .PNG files to the list of files to select from
		if(file.length() > 4 && file.compare(file.length() - 4, 4, ".png") == 0) {
			fileList.push_back(absolutePath + data.cFileName);
		}
	} while(FindNextFileA(hFind, &data));
	FindClose(hFind);

	if(fileList.size() < 1) {
		std::string errorMessage = "No .png files could be found in the given directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
	}

	std::vector<std::string> uniqueList = fileList;
	imageList = new std::string[imageNum];
	int totalBackgrounds = fileList.size();
	srand(time(NULL));

	//Fills the imageList array with random file paths from fileList. 
	//Duplicate files may be included in final list of images, but only after every image has gotten added once. 
	for(int i = 0; i < imageNum; ++i) {
		int uniqueRemaining = uniqueList.size();
		if(uniqueRemaining > 0) {
			int randomIndex = rand() % uniqueRemaining;
			imageList[i] = uniqueList[randomIndex];
			uniqueList.erase(uniqueList.begin() + randomIndex);
		}
		else {
			int randomIndex = rand() % totalBackgrounds;
			imageList[i] = fileList[randomIndex];
		}
	}
}

int imageCount(std::string absolutePath, std::vector<std::string>& fileList) {
	absolutePath.append("/");
	std::string searchPath = absolutePath;
	searchPath.append("*");
	WIN32_FIND_DATAA data;

	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &data);
	if(hFind == INVALID_HANDLE_VALUE) {
		std::string errorMessage = "Could not find given directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
	}

	//Retrieves all files from directory and add thir paths to the fileList
	do {
		std::string file = data.cFileName;
		//Only adds .PNG files to the list of files to select from
		if(file.length() > 4 && file.compare(file.length() - 4, 4, ".png") == 0) {
			fileList.push_back(absolutePath + data.cFileName);
		}
	} while(FindNextFileA(hFind, &data));
	FindClose(hFind);

	if(fileList.size() < 1) {
		std::string errorMessage = "No .png files could be found in the given directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
	}

	return fileList.size();
}

// Verifies atleast one png exists in directory specified by the absolute path.
bool verifyPngsExist(std::string absolutePath) {
	absolutePath.append("/");
	std::string searchPath = absolutePath;
	searchPath.append("*");
	WIN32_FIND_DATAA data;

	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &data);
	if(hFind == INVALID_HANDLE_VALUE) {
		std::string errorMessage = "Could not open selected directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
	}

	// Checks every file in the directory until a png is found.
	do {
		std::string file = data.cFileName;
		//Only adds .PNG files to the list of files to select from
		if(file.length() > 4 && file.compare(file.length() - 4, 4, ".png") == 0) {
			FindClose(hFind);
			return true;
		}
	} while(FindNextFileA(hFind, &data));
	FindClose(hFind);
	return false;
}

std::string createUniqueImageId(const std::string& destinationPath) {
	LPSYSTEMTIME timeinfo = new SYSTEMTIME();
	GetLocalTime(timeinfo);

	// Creates timestamp in format mm-dd-yyyy-hr-min-ss-mls
	std::string timestamp = std::to_string(timeinfo->wMonth) + '-' + std::to_string(timeinfo->wDay) + '-' + std::to_string(timeinfo->wYear) + '-' + std::to_string(timeinfo->wHour) + '-' + std::to_string(timeinfo->wMinute) + '-' + std::to_string(timeinfo->wSecond) + '-' + std::to_string(timeinfo->wMilliseconds);
	std::string imagePath = destinationPath + "/processed" + '-' + timestamp + ".png";
	return imagePath;
}
