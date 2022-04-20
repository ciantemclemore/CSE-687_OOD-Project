#include "pch.h"
#include "regex"
#include "../CSE-687_OOD-Project/FileManagement.cpp"

class FileManagementDeathTest : public ::testing::Test {
public:
	FileManagement fileManager;
};

class FileManagementNormalTest : public ::testing::Test {
public:
	FileManagement fileManager;
	std::vector<std::filesystem::path> GetFiles(const std::string& directory) const {
		std::vector<std::filesystem::path> files;
		for (const auto& file : std::filesystem::directory_iterator(directory))
			files.push_back(file.path());

		return files;
	}
};

TEST_F(FileManagementDeathTest, GetFilesInDirectoryNormalExitTest) {
	const std::string emptyDirectory = "";
	const std::string unknownDirectory = "/Unknown/UnknownDirectory/";
	EXPECT_EXIT(fileManager.GetFilesInDirectory(emptyDirectory), testing::ExitedWithCode(0), "");
	EXPECT_EXIT(fileManager.GetFilesInDirectory(unknownDirectory), testing::ExitedWithCode(0), "");	
}

TEST_F(FileManagementDeathTest, GetFileLinesNormalExitTest) {
	const std::string file = "/Unknow/UnknowDirectory/Unknown.txt";
	EXPECT_EXIT(fileManager.GetFileLines(file), testing::ExitedWithCode(0), "");
}

TEST_F(FileManagementNormalTest, GetFilesInDirectoryTest) {
	auto firstFiles = GetFiles(std::filesystem::current_path().string());
	EXPECT_EQ(firstFiles.size(), fileManager.GetFilesInDirectory(std::filesystem::current_path().string()).size());
}

TEST_F(FileManagementNormalTest, GetFilesLinesTest) {
	auto files = GetFiles(std::filesystem::current_path().string());
	EXPECT_GE(fileManager.GetFileLines(files[0]).size(), 0);
}

