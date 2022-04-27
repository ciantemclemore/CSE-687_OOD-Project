#include "pch.h"
#include "../CSE-687_OOD-Project/Reducer.cpp"

class ReduceAndOutputTest : public ::testing::Test {
public:
	ReduceAndOutputTest() {
		testContainer["the"] = { 1, 1, 1 };
		testContainer["and"] = { 1, 1 };
		testContainer["who"] = { 1 };
		testContainer["what"] = { 1, 1 };
		testContainer["when"] = { 1, 1, 1 };
		testContainer["where"] = { 1, 1, 1, 1 };
		testContainer["why"] = { 1, 1, 1, 1, 1 };
		testContainer["how"] = { 1, 1, 1, 1, 1, 1 };
	}
	FileManagement fileManager;
	std::list<std::filesystem::path> paths;
	std::map<std::string, std::vector<int>> testContainer;
};

TEST_F(ReduceAndOutputTest, ReduceTempAndOutputFinal) {
	Reducer reducer1("../CSE-687_OOD-Project/outputfiles/");

	for (const auto& sortedPair : testContainer) { //call premade map of words and instances
		reducer1.Reduce(sortedPair.first, sortedPair.second); //takes string of key, and vector of integers of iterations

		// test if the final output file was created with correct format
		std::filesystem::path outputPath = "../CSE-687_OOD-Project/outputfiles/"; // + path.filename().string();
		EXPECT_EQ(std::filesystem::exists(outputPath), true) << "Final Output Failed";
	}
}