#include "pch.h"
#include "regex"
#include "../CSE-687_OOD-Project/Utility.h"

class Utilities : public ::testing::Test {
protected:
	void SetUp() override {
		input1 = "";
		input2 = "The brown fox jumped the fence";
		input3 = "There's";
		input4 = "The!re";
		input5 = "T.here";
	}
public:
	std::string input1;
	std::string input2;
	std::string input3;
	std::string input4;
	std::string input5;
};

TEST_F(Utilities, UtilityTestSplitAndClean) {
	auto x = Utility::SplitAndClean(input1);
	EXPECT_EQ(x, std::list<std::string>());
	auto y = Utility::SplitAndClean(input2);
	EXPECT_EQ(y.size(), 6);
}

TEST_F(Utilities, UtilityTestRemoveSpecialCharacters) {
	auto x = Utility::RemoveSpecialCharacters(input3);
	auto y = Utility::RemoveSpecialCharacters(input4);
	auto z = Utility::RemoveSpecialCharacters(input5);
	EXPECT_FALSE(std::regex_search(x, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
	EXPECT_FALSE(std::regex_search(y, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
	EXPECT_FALSE(std::regex_search(z, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
}