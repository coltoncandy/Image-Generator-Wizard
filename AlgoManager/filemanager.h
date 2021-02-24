#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

void getRandomImages(int backgroundNum, std::string absolutePath, std::string*& backgroundList);
int imageCount(std::string absolutePath, std::vector<std::string>& fileList);
bool verifyPngsExist(std::string absolutePath);
std::string createUniqueImageId(const std::string& destinationPath);
