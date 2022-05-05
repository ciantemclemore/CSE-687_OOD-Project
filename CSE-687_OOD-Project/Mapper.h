#pragma once
#include "filesystem"
#include "string"
#include "Utilities.h"

class Mapper
{
private:
	std::list<std::string> writeBuffer;
	std::string currentFileName;
	const std::string outputFilePath;
	size_t totalBufferCount;
public:
	explicit Mapper(const std::string& outputFilePath);
	void Map(const std::filesystem::path& filePath, const std::string& line);
	void ExportData(const std::filesystem::path& filePath, const std::string& token = "");
};

