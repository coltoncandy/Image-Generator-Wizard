#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

void getRandomImages(int backgroundNum, std::string absolutePath, std::string*& backgroundList);
std::string createUniqueImageId(const std::string& destinationPath);

	

