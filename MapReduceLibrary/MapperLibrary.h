#pragma once
#include "string"
#include "filesystem"

//#ifdef MAPPERLIBRARY_EXPORTS
//#define MAPPERLIBRARY_EXPORTS __declspec(dllexport)
//#else
//#define MAPPERLIBRARY_EXPORTS __declspec(dllimport)
//#endif

extern "C" __declspec(dllexport) void Map(const std::filesystem::path & filePath, const std::string & line, const std::filesystem::path & tempOutputPath);
void ExportData(const std::filesystem::path & filePath, const std::vector<std::string>& tokens, const std::filesystem::path& tempOutputPath);