#include "gtest/gtest.h"
#include "../AlgoManager/algomanager.h"

//This is just an example test. It should be deleted once actual tests are added
TEST(TestSuiteName, TestName) {
  AlgoManager::AlgoManager test;
  test.testFunction();
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}