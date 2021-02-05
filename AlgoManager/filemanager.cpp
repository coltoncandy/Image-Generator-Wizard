#include "fileManager.h"

void getRandomBackgrounds(int backgroundNum, std::string absolutePath, std::string *& backgroundList) {
	if(backgroundNum < 1) {
		std::string errorMessage = "The number of background selected to return must be greater than 0.";
		throw errorMessage;
	}

	if(backgroundList) {
		delete[] backgroundList;
		backgroundList = nullptr;
	}

	absolutePath.append("\\");
	std::string searchPath = absolutePath;
	searchPath.append("\\*");
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
			std::cout << file << std::endl;
			fileList.push_back(absolutePath + data.cFileName);
		}
	} while(FindNextFileA(hFind, &data));
	FindClose(hFind);


	if(fileList.size() < 1) {
		std::string errorMessage = "No .png files could be found in the given directory: ";
		errorMessage.append(absolutePath);
		throw errorMessage;
	}

	backgroundList = new std::string[backgroundNum];
	int totalBackgrounds = fileList.size();
	srand(time(NULL));

	//Fills the backgroundList array with random file paths from fileList. Duplicate files may be included in final list of backgrounds. 
	for(int i = 0; i < backgroundNum; ++i) {
		int randomIndex = rand() % totalBackgrounds;
		backgroundList[i] = fileList[randomIndex];
	}
}
