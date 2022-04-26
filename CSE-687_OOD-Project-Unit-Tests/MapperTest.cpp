#include "pch.h"
#include "../CSE-687_OOD-Project/Mapper.cpp"

class MapAndExportTest : public ::testing::Test {
public:
	MapAndExportTest() {
		paths = fileManager.GetFilesInDirectory("../CSE-687_OOD-Project/inputfiles/");
	}
	FileManagement fileManager;
	std::list<std::filesystem::path> paths;
};


TEST_F(MapAndExportTest, TestMapAndExportData) {
	
	Mapper mapper1("../CSE-687_OOD-Project/tempfiles/");
	
	for (const std::filesystem::path& path : paths) {
		std::list<std::string> fileLines = fileManager.GetFileLines(path);

		for (const std::string& line : fileLines) {
			mapper1.Map(path, line);
			
			// test if the intermediate file was created and its data
			std::filesystem::path outputPath = "../CSE-687_OOD-Project/tempfiles/" + path.filename().string();
			EXPECT_EQ(std::filesystem::exists(outputPath), true) << "File Creation did not take place in ExportData function";
		}
	}
}