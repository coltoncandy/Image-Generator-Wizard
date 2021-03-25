#include "gtest/gtest.h"
#include "../AlgoManager/algomanager.h"
#include "opencv2/imgcodecs.hpp"
#include <Windows.h>

class AlgoManagerTests : public ::testing::Test {
protected:
	AlgoManagerTests() {
		//Creating a directory to hold test images
		DWORD nBufferLength = GetCurrentDirectoryA(0, NULL);
		if(nBufferLength) {
			char* cwd = new char[nBufferLength];
			GetCurrentDirectoryA(nBufferLength, cwd);
			testImageDirectory = cwd;
			testImageDirectory.append("/testingDirectory");
			CreateDirectoryA(testImageDirectory.c_str(), NULL);

			//Creating Black and White test PNGs
			blackTestImage = cv::Mat::zeros(480, 640, CV_8UC4);
			whiteTestImage = cv::Mat::ones(480, 640, CV_8UC4)* 255;
			delete[] cwd;
		}
	}

	virtual void TearDown() {
		RemoveDirectoryA(testImageDirectory.c_str());
	}

	std::string testImageDirectory;
	cv::Mat blackTestImage;
	cv::Mat whiteTestImage;
};

TEST_F(AlgoManagerTests, TestIfNoPathIsGivenGrabCutThrowsErrorMessage) {
	AlgoManager::AlgoManager testManager;
	try {
		testManager.grabCutWrapper("");
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, "Failed to load image into grabCut: No image path was provided.");
	}
	catch(...) {
		FAIL() << "Threw Unexpected error";
	}
}

TEST_F(AlgoManagerTests, TestIfInvalidTargetPathIsGivenProcessThrowsError) {
	std::string backgroundPath = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(backgroundPath, blackTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.process("", backgroundPath);
		DeleteFileA(backgroundPath.c_str());
		FAIL();
	}
	catch(int ex) {
		EXPECT_EQ(ex, -1);
		DeleteFileA(backgroundPath.c_str());
	}
	catch(...) {
		DeleteFileA(backgroundPath.c_str());
		FAIL();
	}
}

TEST_F(AlgoManagerTests, TestIfInvalidBackgroundPathIsGivenProcessThrowsError) {
	std::string targetPath = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(targetPath, blackTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.process(targetPath, "");
		DeleteFileA(targetPath.c_str());
		FAIL();
	}
	catch(int ex) {
		EXPECT_EQ(ex, 1);
		DeleteFileA(targetPath.c_str());
	}
	catch(...) {
		DeleteFileA(targetPath.c_str());
		FAIL();
	}
}

TEST_F(AlgoManagerTests, TestIfValidPathsAreGivenProcessReturnsNewImage) {
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPath = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(targetPath, whiteTestImage);
	cv::imwrite(backgroundPath, blackTestImage);
	AlgoManager::AlgoManager testManager;

	cv::Mat returnedImage = testManager.process(targetPath, backgroundPath);
	EXPECT_FALSE(returnedImage.empty());
	DeleteFileA(targetPath.c_str());
	DeleteFileA(backgroundPath.c_str());
}

TEST_F(AlgoManagerTests, TestIfEmptyMatrixIsGivenProcessTargetThrowsError) {
	AlgoManager::AlgoManager testManager;
	try {
		cv::Mat emptyMatrix;
		testManager.processTarget(emptyMatrix);
		FAIL();
	}
	catch(int ex) {
		EXPECT_EQ(ex, -1);
	}
	catch(...) {
		FAIL();
	}
}


TEST_F(AlgoManagerTests, TestIfValidMatrixIsGivenProcessTargetReturnsNewMatix) {
	AlgoManager::AlgoManager testManager;
	cv::Mat returnedImage = testManager.processTarget(blackTestImage);
	EXPECT_FALSE(returnedImage.empty());
}

TEST_F(AlgoManagerTests, TestIfEmptyBackgroundIsGivenProcessBackgroundThrowsError) {
	AlgoManager::AlgoManager testManager;
	try {
		cv::Mat emptyMatrix;
		testManager.processBackground(emptyMatrix, whiteTestImage);
		FAIL();
	}
	catch(int ex) {
		EXPECT_EQ(ex, 1);
	}
	catch(...) {
		FAIL();
	}
}

TEST_F(AlgoManagerTests, TestIfEmptyTargetIsGivenProcessBackgroundThrowsError) {
	AlgoManager::AlgoManager testManager;
	try {
		cv::Mat emptyMatrix;
		testManager.processBackground(whiteTestImage, emptyMatrix);
		FAIL();
	}
	catch(int ex) {
		EXPECT_EQ(ex, -1);
	}
	catch(...) {
		FAIL();
	}
}

TEST_F(AlgoManagerTests, TestIfValidMatricesAreGivenProcessBackgroundReturnsNewMatix) {
	AlgoManager::AlgoManager testManager;
	cv::Mat returnedImage = testManager.processBackground(blackTestImage, whiteTestImage);
	EXPECT_FALSE(returnedImage.empty());
}

TEST_F(AlgoManagerTests, TestInvalidImageNumThrowsError) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPath = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(targetPath, whiteTestImage);
	cv::imwrite(backgroundPath, blackTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.batchProcess(0, targetPath, &backgroundPath, imageBatch);
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPath.c_str());
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, "The number of images to generate must be greater than 0.");
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPath.c_str());
	}
	catch(...) {
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPath.c_str());
		FAIL() << "Threw Unexpected error";
	}
}

TEST_F(AlgoManagerTests, TestNullBackgroundPathsPointerThrowsError) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string* backgroundPaths = nullptr;
	cv::imwrite(targetPath, whiteTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.batchProcess(1, targetPath, backgroundPaths, imageBatch);
		DeleteFileA(targetPath.c_str());
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, "No background paths were provided to batchProcessing().");
		DeleteFileA(targetPath.c_str());
	}
	catch(...) {
		DeleteFileA(targetPath.c_str());
		FAIL() << "Threw Unexpected error";
	}
}

TEST_F(AlgoManagerTests, TestInvalidTargetPathThrowsError) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = "C:/Invalid/Path";
	std::string backgroundPath = testImageDirectory + "/blackTestImage.png";
	std::string expectedError = "An error occured while accessing the target image (" + targetPath + "): 0/1 images were generated.";
	cv::imwrite(backgroundPath, blackTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.batchProcess(1, targetPath, &backgroundPath, imageBatch);
		DeleteFileA(backgroundPath.c_str());
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, expectedError);
		DeleteFileA(backgroundPath.c_str());
	}
	catch(...) {
		DeleteFileA(backgroundPath.c_str());
		FAIL() << "Threw Unexpected error";
	}
}

TEST_F(AlgoManagerTests, TestIfValidArgumentsGivenBatchProcessGeneratesImages) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPaths[2];
	backgroundPaths[0] = testImageDirectory + "/blackTestImage.png";
	backgroundPaths[1] = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(targetPath, whiteTestImage);
	cv::imwrite(backgroundPaths[0], blackTestImage);
	AlgoManager::AlgoManager testManager;
	testManager.batchProcess(2, targetPath, backgroundPaths, imageBatch);
	EXPECT_EQ(imageBatch.size(), 2);
	DeleteFileA(targetPath.c_str());
	DeleteFileA(backgroundPaths[0].c_str());
}

TEST_F(AlgoManagerTests, TestIfImageBatchIsNotEmptyBatchProcessClearsIt) {
	std::vector <cv::Mat> imageBatch;
	imageBatch.push_back(blackTestImage);
	imageBatch.push_back(blackTestImage);
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPath = testImageDirectory + "/blackTestImage.png";
	cv::imwrite(targetPath, whiteTestImage);
	cv::imwrite(backgroundPath, blackTestImage);
	AlgoManager::AlgoManager testManager;
	testManager.batchProcess(1, targetPath, &backgroundPath, imageBatch);
	EXPECT_EQ(imageBatch.size(), 1);
	DeleteFileA(targetPath.c_str());
	DeleteFileA(backgroundPath.c_str());
}

TEST_F(AlgoManagerTests, TestIfInvalidBackgroundPathsGivenBatchProcessThrowsErrorMessageWithInvalidPaths) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPaths[2];
	backgroundPaths[0] = "C:/Invalid/Path/1";
	backgroundPaths[1] = "C:/Invalid/Path/2";
	std::string expectedError = "Errors occured while accessing the following background images:\n" + backgroundPaths[0] + "\n" + backgroundPaths[1] + "\n\n0/2 images were generated.";
	cv::imwrite(targetPath, whiteTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.batchProcess(2, targetPath, backgroundPaths, imageBatch);
		DeleteFileA(targetPath.c_str());
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, expectedError);
		DeleteFileA(targetPath.c_str());
	}
	catch(...) {
		DeleteFileA(targetPath.c_str());
		FAIL() << "Threw Unexpected error";
	}
}

TEST_F(AlgoManagerTests, TestIfOneInvalidBackgroundPathIsGivenBatchProcessGeneratesImagesWithValidPathsThenThrowsErrorMessageWithInvalidPaths) {
	std::vector <cv::Mat> imageBatch;
	std::string targetPath = testImageDirectory + "/whiteTestImage.png";
	std::string backgroundPaths[3];
	backgroundPaths[0] = testImageDirectory + "/blackTestImage.png";
	backgroundPaths[1] = "C:/Invalid/Path";
	backgroundPaths[2] = testImageDirectory + "/blackTestImage.png";
	std::string expectedError = "Errors occured while accessing the following background images:\n" + backgroundPaths[1] + "\n\n2/3 images were generated.";
	cv::imwrite(targetPath, whiteTestImage);
	cv::imwrite(backgroundPaths[0], blackTestImage);
	AlgoManager::AlgoManager testManager;
	try {
		testManager.batchProcess(3, targetPath, backgroundPaths, imageBatch);
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPaths[0].c_str());
		FAIL() << "Did not throw an error";
	}
	catch(std::string ex) {
		EXPECT_EQ(ex, expectedError);
		EXPECT_EQ(imageBatch.size(), 2);
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPaths[0].c_str());
	}
	catch(...) {
		DeleteFileA(targetPath.c_str());
		DeleteFileA(backgroundPaths[0].c_str());
		FAIL() << "Threw Unexpected error";
	}
}


