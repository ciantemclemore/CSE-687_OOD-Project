#include "pch.h"
#include "../CSE-687_OOD-Project/FileManagement.h"

class FileManagementNormalTest : public ::testing::Test {
public:
	std::vector<std::filesystem::path> GetFiles(const std::string& directory) const {
		std::vector<std::filesystem::path> files;
		for (const auto& file : std::filesystem::directory_iterator(directory))
			files.push_back(file.path());

		return files;
	}
};

TEST(FileManagementDeathTest, GetFilesInDirectoryNormalExitTest) {
	const std::string emptyDirectory = "";
	const std::string unknownDirectory = "/Unknown/UnknownDirectory/";
	EXPECT_EXIT(FileManagement::GetFilesInDirectory(emptyDirectory), testing::ExitedWithCode(0), "");
	EXPECT_EXIT(FileManagement::GetFilesInDirectory(unknownDirectory), testing::ExitedWithCode(0), "");
}

TEST(FileManagementDeathTest, GetFileLinesNormalExitTest) {
	const std::string file = "/Unknow/UnknowDirectory/Unknown.txt";
	EXPECT_EXIT(FileManagement::GetFileLines(file), testing::ExitedWithCode(0), "");
}

TEST_F(FileManagementNormalTest, GetFilesInDirectoryTest) {
	auto firstFiles = GetFiles(std::filesystem::current_path().string());
	EXPECT_EQ(firstFiles.size(), FileManagement::GetFilesInDirectory(std::filesystem::current_path().string()).size());
}

TEST_F(FileManagementNormalTest, GetFilesLinesTest) {
	auto files = GetFiles(std::filesystem::current_path().string());
	EXPECT_GE(FileManagement::GetFileLines(files[0]).size(), 0);
}

