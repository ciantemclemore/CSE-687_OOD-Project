#include "pch.h"
#include "framework.h"
#include "MapperLibrary.h"
#include "ReducerLibrary.h"
#include "Utilities.h"

__declspec(dllexport) void Map(const std::filesystem::path& filePath, const std::string& line, const std::filesystem::path& tempOutputPath) {
	auto tokens = Utilities::SplitAndClean(line);
	ExportData(filePath, tokens, tempOutputPath);
}

__declspec(dllexport) void Reduce(const std::string& key, const std::vector<int>& iterations, const std::filesystem::path& outputFilePath) {

	// Sum up the iterations
	int totalCount = 0;
	for (int i = 0; i < iterations.size(); i++) {
		totalCount += iterations[i];
	}
	ExportData(key, totalCount, outputFilePath);
}

/// <summary>
/// The mapper export function will take the tokens and write them to intermediate files
/// </summary>
/// <param name="filePath"></param>
/// <param name="tokens"></param>
/// <param name="tempOutputPath"></param>
void ExportData(const std::filesystem::path& filePath, const std::vector<std::string>& tokens, const std::filesystem::path& tempOutputPath) {
	
	std::list<std::string> writeBuffer;

	for (const auto& token : tokens) {
		writeBuffer.push_back(token + " 1");
	}

	// write the buffer to the file
	if (std::string tempFileName = tempOutputPath.string() + "\\" + filePath.filename().string(); std::filesystem::exists(tempFileName)) {
		Utilities::WriteBufferToFile(writeBuffer, tempFileName, std::ios::app);
	}
	else {
		Utilities::WriteBufferToFile(writeBuffer, tempFileName, std::ios::out);
	}

	// clear the buffer after writing
	writeBuffer.clear();
}


/// <summary>
/// The reducer export function will take two parameters. A string key and the resulting reduced data from
/// the reduce function. The function will write the result to the output directory. This function should
/// not directly deal with an File IO.
/// </summary>
void ExportData(const std::string& key, int reducedData, const std::filesystem::path& outputFilePath) { //reduced data
	std::list<std::string> buffer = std::list<std::string>();
	std::string content = key + " " + std::to_string(reducedData);
	buffer.push_back(content);
	Utilities::WriteBufferToFile(buffer, outputFilePath.string() + "\\" + "finalOutput.txt", std::ios::app);
}
