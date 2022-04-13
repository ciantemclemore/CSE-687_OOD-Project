#include "FileManagement.h"

/// <summary>
/// Constructor
/// </summary>
FileManagement::FileManagement() = default;

std::list<std::filesystem::path> FileManagement::GetFilesInDirectory(const std::string& directory) const {

	std::list<std::filesystem::path> files;

	if (!directory.empty()) {
		for (const auto& entry : std::filesystem::directory_iterator(directory)) {
			files.push_back(entry.path());
		}
	}

	return files;
}

void FileManagement::ReadFileByLine(const std::filesystem::path& path) const {
	
	std::fstream file;
	file.open(path, std::ios::in);
	
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::cout << line << std::endl;
		}
		file.close();
	}
}