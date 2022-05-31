#include "pch.h"
#include "fstream"
#include "../MapReduceLibrary/ReducerLibrary.h"

class ReduceAndOutputTest : public ::testing::Test {
public:
	ReduceAndOutputTest() {
		testContainer.emplace_back("the");
		testContainer.emplace_back("and");
		testContainer.emplace_back("who");
		testContainer.emplace_back("what");
		testContainer.emplace_back("when");
		testContainer.emplace_back("where");
		testContainer.emplace_back("why");
		testContainer.emplace_back("how");
		testContainer.emplace_back("when");
		testContainer.emplace_back("then");
	}
	std::list<std::filesystem::path> paths;
	std::vector<std::string> testContainer;
};

TEST_F(ReduceAndOutputTest, ReduceTempAndOutputFinal) {

	std::vector<std::filesystem::path> files;
	std::fstream file;
	file.open("../CSE-687_OOD-Project/outputfiles/tempFinalOutput.txt", std::ios::app);

	if (file.is_open()) {

		for (const auto& key : testContainer) {
			file << key << " " << 1 << std::endl;
		}
		files.emplace_back("../CSE-687_OOD-Project/outputfiles/tempFinalOutput.txt");
		file.close();
	}

	auto totalFiles = files.size();

	Reduce(files, totalFiles, "../CSE-687_OOD-Project/outputfiles/"); //takes string of key, and vector of integers of iterations

	// test if the final output file was created with correct format
	std::filesystem::path outputPath = "../CSE-687_OOD-Project/outputfiles/finalOutput.txt";
	EXPECT_EQ(std::filesystem::exists(outputPath), true) << "Final Output Failed";
	remove(outputPath);
}