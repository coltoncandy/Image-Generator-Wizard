#include "gtest/gtest.h"
#include "../AlgoManager/algomanager.h"
#include <regex>

/*
TEST(AlgoManagerTests, TestThatIfAnInvalidPathIsGivenToGrabCutAnErrorMessageIsDisplayed) {
	AlgoManager::AlgoManager testManager;
	testing::internal::CaptureStdout();
	testManager.grabCutWrapper("C:/Non/Existant/Path");
	std::string stdOutput = testing::internal::GetCapturedStdout();
	EXPECT_EQ("\n Could not read file path\n" , stdOutput);
}
*/

TEST(AlgoManagerTests, TestCreateUniqueImageIdFormatsPathAsExpected) {
	AlgoManager::AlgoManager testManager;
	std::string returnedPath =  testManager.createUniqueImageId("C:/Base/Path");
	std::regex pathRegEx("C:/Base/Path/processed-[0-9][0-9]?-[0-9][0-9]?-[0-9][0-9][0-9][0-9]-[0-9][0-9]?-[0-9][0-9]?-[0-9][0-9]?-[0-9][0-9][0-9]?.png");
	EXPECT_TRUE(std::regex_match(returnedPath.c_str(), pathRegEx));
}
