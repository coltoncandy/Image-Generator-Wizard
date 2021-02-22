#include "fileManager.h"

//Randomly fills the array of strings, imageList, with paths of images found in the directory
//specified by the absolutePath. The number of images added to imageList is set by the imageNum parameter.
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

	imageList = new std::string[imageNum];
	int totalBackgrounds = fileList.size();
	srand(time(NULL));

	//Fills the imageList array with random file paths from fileList. Duplicate files may be included in final list of images. 
	for(int i = 0; i < imageNum; ++i) {
		int randomIndex = rand() % totalBackgrounds;
		imageList[i] = fileList[randomIndex];
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
