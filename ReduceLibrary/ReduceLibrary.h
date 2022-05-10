// ReduceLibrary.h - Contains declarations of reduce functions
#pragma once
#include "filesystem"
#include "string"
#include "numeric"

//#ifdef REDUCELIBRARY_EXPORTS
//#define REDUCELIBRARY_API __declspec(dllexport)
//#else
//#define REDUCELIBRARY_API __declspec(dllimport)
//#endif
//#if __cplusplus
//extern "C" {
//#endif

__declspec(dllexport) void Reduce(const std::string & key, const std::vector<int>&iterations, const std::string& outputFilePath);
void ExportData(const std::string& key, const int reducedData, const std::string& outputFilePath);

//#if __cplusplus
//}
//#endif