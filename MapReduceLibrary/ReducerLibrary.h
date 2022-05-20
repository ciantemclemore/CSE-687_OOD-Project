// ReduceLibrary.h - Contains declarations of reduce functions
#pragma once
#include "filesystem"
#include "string"
#include "map"
#include "mutex"

extern "C" __declspec(dllexport) void Reduce(const std::vector<std::filesystem::path>&filePaths, int totalFileCount, const std::filesystem::path& outputFilePath);
void ExportData(const std::string& key, const int reducedData, const std::filesystem::path& outputFilePath);

