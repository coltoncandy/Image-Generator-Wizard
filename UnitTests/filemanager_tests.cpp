#include "gtest/gtest.h"
#include "../AlgoManager/filemanager.h"
#include "opencv2/imgcodecs.hpp"

class FileManagerTests : public ::testing::Test {
protected:
	FileManagerTests() {
		returnedImages = nullptr;

		//Creating a directory to hold test images
		DWORD nBufferLength = GetCurrentDirectoryA(0, NULL);
		if(nBufferLength) 
		{
			char* cwd = new char[nBufferLength];
			GetCurrentDirectoryA(nBufferLength, cwd);
			testImageDirectory = cwd;
			testImageDirectory.append("/testingDirectory");
			CreateDirectoryA(testImageDirectory.c_str(), NULL);

			//Creating test PNG files
			testImage = cv::Mat::zeros(480, 640, CV_8UC4);
			delete[] cwd;
		}
	}

	virtual ~FileManagerTests() {
		if(returnedImages)
			delete[] returnedImages;
	}

	virtual void TearDown() {
		RemoveDirectoryA(testImageDirectory.c_str());
	}

	std::string testImageDirectory;
	std::string* returnedImages;
	cv::Mat testImage;
};
TEST_F(FileManagerTests, InvalidPathThrowsErrorMessage) {
	std::string invalidPath = "C:/Non/Existant/Path";
	EXPECT_THROW(getRandomImages(1, invalidPath, returnedImages), std::string);
	EXPECT_EQ(nullptr, returnedImages);
}
 TEST_F(FileManagerTests, InvalidBackgroundNumberThrowsErrorMessage) {
	EXPECT_THROW(getRandomImages(0, testImageDirectory, returnedImages), std::string);
	EXPECT_EQ(nullptr, returnedImages);
}

 TEST_F(FileManagerTests, NoFoundPngFilesInGivenDirectoryThrowsErrorMessage) {
	EXPECT_THROW(getRandomImages(1, testImageDirectory, returnedImages), std::string);
	EXPECT_EQ(nullptr, returnedImages);
}

TEST_F(FileManagerTests, RequestingOneImageSetsAnImagePath) {
	std::string filePath = testImageDirectory + "/testImage.png";
	imwrite(filePath, testImage);
	getRandomImages(1, testImageDirectory, returnedImages);
	ASSERT_NE(nullptr, returnedImages);
	EXPECT_EQ(returnedImages[0], filePath);
	DeleteFileA(filePath.c_str());
}

TEST_F(FileManagerTests, RequestingManyImagesWithOnlyOnePngExistingSetsSameImagePathMultipleTimes) {
	std::string filePath = testImageDirectory + "/testImage.png";
	imwrite(filePath, testImage);
	getRandomImages(5, testImageDirectory, returnedImages);
	ASSERT_NE(nullptr, returnedImages);
	for(int i = 0; i < 5; ++i) {
		EXPECT_EQ(returnedImages[i], filePath);
	}
	DeleteFileA(filePath.c_str());
}

TEST_F(FileManagerTests, AlreadyInitializedPointerDeletedAndGetsReinitialized) {
	returnedImages = new std::string[] {"C:/Previously/Returned/Path"};
	std::string filePath = testImageDirectory + "/testImage.png";
	imwrite(filePath, testImage);

	getRandomImages(3, testImageDirectory, returnedImages);
	ASSERT_NE(nullptr, returnedImages);
	for(int i = 0; i < 3; ++i) {
		EXPECT_EQ(returnedImages[i], filePath);
	}
	DeleteFileA(filePath.c_str());
}

TEST_F(FileManagerTests, requestingManyImagesWithManyPngsExistingSetsMultipleDifferentPaths) {
	int imageNum = 20;
	std::string filePath1 = testImageDirectory + "/testImage1.png";
	imwrite(filePath1, testImage);
	std::string filePath2 = testImageDirectory + "/testImage2.png";
	imwrite(filePath2, testImage);

	getRandomImages(imageNum, testImageDirectory, returnedImages);
	ASSERT_NE(nullptr, returnedImages);

	int file1Count = 0;
	int file2Count = 0;
	for(int i = 0; i < imageNum; ++i) {
		if(filePath1.compare(returnedImages[i]))
			++file1Count;
		else if(filePath2.compare(returnedImages[i]))
			++file2Count;
	}
	EXPECT_GT(file1Count, 0);
	EXPECT_GT(file2Count, 0);
	EXPECT_EQ(file1Count + file2Count, imageNum);

	DeleteFileA(filePath1.c_str());
	DeleteFileA(filePath2.c_str());
}
