// ReduceLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h"
#include "framework.h"
#include "ReduceLibrary.h"
#include "Utilities.h"
#include <list>

__declspec(dllexport) void Reduce(const std::string& key, const std::vector<int>& iterations) {

	// Sum up the iterations
	int totalCount = 0;
	for (int i = 0; i < iterations.size(); i++) {
		totalCount += iterations[i];
	}
	ExportData(key, totalCount);
}

/// <summary>
/// The export function will take two parameters. A string key and the resulting reduced data from
/// the reduce function. The function will write the result to the output directory. This function should
/// not directly deal with an File IO.
/// </summary>
void ExportData(const std::string& key, int reducedData) { //reduced data
	std::list<std::string> buffer = std::list<std::string>();
	std::string content = key + " " + std::to_string(reducedData);
	buffer.push_back(content);
	Utilities::WriteBufferToFile(buffer, outputFilePath + "\\" + "finalOutput.txt", std::ios::app);
}