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
	FileManagement();
	std::list<std::filesystem::path> GetFilesInDirectory(const std::string& directory) const;
	std::list<std::string> GetFileLines(const std::filesystem::path& path) const;
	void WriteBufferToFile(const std::list<std::string>& buffer, const std::string& path, std::ios_base::openmode mode) const;
};

