#include "pch.h"
#include "../MapReduceLibrary/ReducerLibrary.h"

class ReduceAndOutputTest : public ::testing::Test {
public:
	ReduceAndOutputTest() {
		testContainer.emplace("the", std::vector<int>(3, 1));
		testContainer.emplace("and", std::vector<int>(1, 1));
		testContainer.emplace("who", std::vector<int>(2, 1));
		testContainer.emplace("what", std::vector<int>(1, 1));
		testContainer.emplace("when", std::vector<int>(2, 1));
		testContainer.emplace("where", std::vector<int>(3, 1));
		testContainer.emplace("why", std::vector<int>(4, 1));
		testContainer.emplace("how", std::vector<int>(5, 1));
		testContainer.emplace("when", std::vector<int>(6, 1));
		testContainer.emplace("then", std::vector<int>(4, 1));
	}
	std::list<std::filesystem::path> paths;
	std::map<std::string, std::vector<int>> testContainer;
};

//TEST_F(ReduceAndOutputTest, ReduceTempAndOutputFinal) {
//	for (const auto& sortedPair : testContainer) { //call premade map of words and instances
//		Reduce(sortedPair.first, sortedPair.second, "../CSE-687_OOD-Project/outputfiles/"); //takes string of key, and vector of integers of iterations
//	}
//
//	// test if the final output file was created with correct format
//	std::filesystem::path outputPath = "../CSE-687_OOD-Project/outputfiles/finalOutput.txt";
//	EXPECT_EQ(std::filesystem::exists(outputPath), true) << "Final Output Failed";
//	remove(outputPath);
//}