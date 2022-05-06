#pragma once
#include "string"
#include "filesystem"

#ifdef MAPPERLIBRARY_EXPORTS
#define MAPPERLIBRARY_EXPORTS __declspec(dllexport)
#else
#define MAPPERLIBRARY_EXPORTS __declspec(dllimport)
#endif

extern "C" MAPPERLIBRARY_EXPORTS void Map(const std::filesystem::path & filePath, const std::string & line);
extern "C" MAPPERLIBRARY_EXPORTS void ExportData(const std::filesystem::path & filePath, const std::string & token = "");