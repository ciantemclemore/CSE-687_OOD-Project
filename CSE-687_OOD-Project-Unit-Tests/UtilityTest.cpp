#include "pch.h"
#include "regex"
#include "Utilities.h"

class Utilities_F : public ::testing::Test {
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

TEST_F(Utilities_F, UtilityTestSplitAndClean) {
	auto x = Utilities::SplitAndClean(input1);
	EXPECT_EQ(x, std::vector<std::string>());
	auto y = Utilities::SplitAndClean(input2);
	EXPECT_EQ(y.size(), 6);
}

TEST_F(Utilities_F, UtilityTestRemoveSpecialCharacters) {
	auto x = Utilities::RemoveSpecialCharacters(input3);
	auto y = Utilities::RemoveSpecialCharacters(input4);
	auto z = Utilities::RemoveSpecialCharacters(input5);
	EXPECT_FALSE(std::regex_search(x, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
	EXPECT_FALSE(std::regex_search(y, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
	EXPECT_FALSE(std::regex_search(z, std::regex("[^a-zA-Z0-9]+"))) << "The input contains special characters." << std::endl;
}