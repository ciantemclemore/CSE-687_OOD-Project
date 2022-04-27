#pragma once
#include "string"
#include "filesystem"
#include "fstream"
#include "iostream"

/// <summary>
/// The file management class will be responsible for accessing the file system.
/// </summary>
class FileManagement
{
public:
	FileManagement() = default;
	std::vector<std::filesystem::path> static GetFilesInDirectory(const std::string& directory) {

		std::vector<std::filesystem::path> files;

		// Determine if the directory exists or not
		if (!std::filesystem::is_directory(directory)) {
			std::cout << "Directory does not exist, please create directory " << "\"" << directory << "\"" << std::endl;
			exit(0);
		}

		if (!directory.empty()) {
			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				files.push_back(entry.path());
			}
		}

		return files;
	}

	std::vector<std::string> static GetFileLines(const std::filesystem::path& path) {

		std::vector<std::string> fileLines;
		std::fstream file;
		file.open(path, std::ios::in);

		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				fileLines.push_back(line);
			}
			file.close();
		}
		else {
			std::cout << "Path to file does not exist, please create a file at path " << path << std::endl;
			exit(0);
		}

		return fileLines;
	}

	void static WriteBufferToFile(const std::list<std::string>& buffer, const std::string& path, std::ios_base::openmode mode) {

		std::fstream file;
		file.open(path, mode);

		if (file.is_open()) {

			for (const std::string& data : buffer) {
				file << data << std::endl;
			}
			file.close();
		}
	}
};

