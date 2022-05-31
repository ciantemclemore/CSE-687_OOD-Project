#include "pch.h"
#include "framework.h"
#include "MapperLibrary.h"
#include "ReducerLibrary.h"
#include "Utilities.h"

__declspec(dllexport) void Map(const std::vector<std::filesystem::path>& filePaths, const std::filesystem::path& tempOutputPath) {
	
	// Get all the lines in a specified file
	for (const auto& filePath : filePaths) 
	{
		std::vector<std::string> fileLines = Utilities::GetFileLines(filePath);
		
		// Get all the tokens for the each line and begin writing
		for (const std::string& line : fileLines) {
			auto tokens = Utilities::SplitAndClean(line);
			ExportData(filePath, tokens, tempOutputPath);
		}
	}
}

__declspec(dllexport) void Reduce(const std::vector<std::filesystem::path>& filePaths, int totalFileCount, const std::filesystem::path& outputFilePath) {

	// Sort and aggregate the given files
	static std::map<std::string, std::vector<int>, std::less<>> sortContainer;
	static int count = 0;
	static std::mutex s_mutex;
	static std::mutex c_mutex;

	for (const auto& filePath : filePaths)
	{
		std::vector<std::string> fileLines = Utilities::GetFileLines(filePath);

		// Get all the tokens for the each line and begin writing
		for (const std::string& line : fileLines) {
			
			auto tokens = Utilities::Split(line);

			std::string sortKey = tokens[0];
			
			std::lock_guard lock(s_mutex);
			// place the token in the map if it doesn't exist.. else - push to the tokens current vector
			if (!sortContainer.try_emplace(sortKey, 1, 1).second) {
				sortContainer[sortKey].push_back(1);
			}
		}
		{
			std::lock_guard lock(c_mutex);
			count++;
		}
	}

	if (count == totalFileCount) {
		for (const auto& [key, value] : sortContainer)
		{
			ExportData(key, value.size(), outputFilePath);
		}
	}
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
	if (std::filesystem::path tempFileName = tempOutputPath.string() + "\\" + filePath.filename().string(); std::filesystem::exists(tempFileName)) {
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
	std::list<std::string> buffer;
	std::string content = key + " " + std::to_string(reducedData);
	buffer.push_back(content);
	Utilities::WriteBufferToFile(buffer, outputFilePath.string() + "\\" + "finalOutput.txt", std::ios::app);
}
