#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

void getRandomImages(int backgroundNum, std::string absolutePath, std::string*& backgroundList);

int imageCount(std::string absolutePath, std::vector<std::string>& fileList);

