#pragma once
#include <filesystem>
#include "Utility.h"
#include <string>
#include "FileManagement.h"
#include <numeric>

class Reducer
{
private:
	std::list<int> instances;
	std::string currentFileName;
	const std::string outputFilePath;
	FileManagement fileManager;
public:
	Reducer(const std::string& outputFilePath);
	void Reduce(const std::string& key, const std::vector<int>& iterations);
	void ExportData(const std::string& key, const int reducedData);
};