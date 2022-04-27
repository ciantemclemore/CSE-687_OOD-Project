#include "pch.h"
#include "../CSE-687_OOD-Project/FileManagement.h"
#include "../CSE-687_OOD-Project/Mapper.cpp"
#include "../CSE-687_OOD-Project/Sorter.h"

class MapAndSortTest : public ::testing::Test {
public:
	MapAndSortTest() {
		files = FileManagement::GetFilesInDirectory("../CSE-687_OOD-Project/inputfiles/");
	}
	std::vector<std::filesystem::path> files;
};


TEST_F(MapAndSortTest, TestMapAndExportData) {
	
	Mapper mapper("../CSE-687_OOD-Project/tempfiles/");
	std::cout << "Mapping Files..." << std::endl;
	for (const std::filesystem::path& path : files) {
		std::vector<std::string> fileLines = FileManagement::GetFileLines(path);

		for (const std::string& line : fileLines) {
			mapper.Map(path, line);
			
			// test if the intermediate file was created and its data
			std::filesystem::path outputPath = "../CSE-687_OOD-Project/tempfiles/" + path.filename().string();
			EXPECT_EQ(std::filesystem::exists(outputPath), true) << "File Creation did not take place in ExportData function";
		}
		std::filesystem::path fileToRemove = "../CSE-687_OOD-Project/tempfiles/" + path.filename().string();
		remove(fileToRemove);
	}
}


TEST_F(MapAndSortTest, TestSort) {
	
	Mapper mapper("../CSE-687_OOD-Project/tempfiles/");
	Sorter sorter;

	std::filesystem::path testInputFile = files[0];
	std::vector<std::string> testInputFileLines = FileManagement::GetFileLines(testInputFile);

	for (const std::string& line : testInputFileLines) {
		mapper.Map(testInputFile, line);
	}

	// now test sorting on the mapper file
	auto tempFile = FileManagement::GetFilesInDirectory("../CSE-687_OOD-Project/tempfiles/")[0];
	auto tempFileLInesCount = FileManagement::GetFileLines(tempFile).size();
	std::vector<std::filesystem::path> files(1, tempFile);
	auto x = sorter.sortAndAggregate(files);
	EXPECT_LT(x.size(), tempFileLInesCount);
	std::filesystem::path fileToRemove = "../CSE-687_OOD-Project/tempfiles/" + testInputFile.filename().string();
	remove(fileToRemove);
}