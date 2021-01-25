#include "gtest/gtest.h"
#include "../AlgoManager/algomanager.h"

TEST(AlgoManagerTests, TestThatIfAnInvalidPathIsGivenToGrabCutAnErrorMessageIsDisplayed) {
	AlgoManager::AlgoManager testManager;
	testing::internal::CaptureStdout();
	testManager.grabCutWrapper("C:/Non/Existant/Path");
	std::string stdOutput = testing::internal::GetCapturedStdout();
	EXPECT_EQ("\n Could not read file path\n" , stdOutput);
}
