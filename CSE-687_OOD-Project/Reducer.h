#pragma once
#include "filesystem"
#include "string"
#include "Utilities.h"
#include "numeric"

class Reducer
{
private:
	std::list<int> instances;
	std::string currentFileName;
	const std::filesystem::path outputFilePath;
public:
	Reducer(const std::filesystem::path& outputFilePath);
	void Reduce(const std::string& key, const std::vector<int>& iterations);
	void ExportData(const std::string& key, const int reducedData);
};