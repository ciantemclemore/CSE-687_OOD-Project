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

std::list<std::string> FileManagement::GetFileLines(const std::filesystem::path& path) const {
	
	std::list<std::string> fileLines;
	std::fstream file;
	file.open(path, std::ios::in);
	
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			fileLines.push_back(line);
		}
		file.close();
	}

	return fileLines;
}

void FileManagement::WriteBufferToFile(const std::list<std::string>& buffer, const std::string& path, std::ios_base::openmode mode) const {
	
	std::fstream file;
	file.open(path, mode);

	if (file.is_open()) {
		
		for (const std::string& data : buffer) {
			file << data << std::endl;
		}
		file.close();
	}
}