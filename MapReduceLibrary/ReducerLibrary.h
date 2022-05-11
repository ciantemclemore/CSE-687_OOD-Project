// ReduceLibrary.h - Contains declarations of reduce functions
#pragma once
#include "filesystem"
#include "string"

extern "C" __declspec(dllexport) void Reduce(const std::string& key, const std::vector<int>& iterations, const std::filesystem::path& outputFilePath);
void ExportData(const std::string& key, const int reducedData, const std::filesystem::path& outputFilePath);

