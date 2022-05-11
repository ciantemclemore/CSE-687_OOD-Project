#pragma once
#include "string"
#include "filesystem"

extern "C" __declspec(dllexport) void Map(const std::filesystem::path & filePath, const std::string & line, const std::filesystem::path & tempOutputPath);
void ExportData(const std::filesystem::path & filePath, const std::vector<std::string>& tokens, const std::filesystem::path& tempOutputPath);