#include "Mapper.h"

/// <summary>
/// Constructor
/// </summary>
Mapper::Mapper(const std::string& outputFilePath) : outputFilePath(outputFilePath) {
}

/// <summary>
/// The map function will accept two parameters. A key (filename) and a value (single line of raw data).
/// The function will tokenize the value into distinct words and then call export.
/// </summary>
void Mapper::map(const std::filesystem::path& filePath, const std::string& line) {
	
	auto tokens = Utility::SplitAndClean(line);

	for (const std::string& token : tokens) {
		exportData(filePath, token);
	}
	
	// call export data one last time to ensure you write the final file
	exportData(filePath);
}

/// <summary>
/// The export function accept two parameters. A key (filename) and a value (formatted/tokenized line of data).
/// The function will buffer output in memory and periodically write the data out to disk.
/// </summary>
void Mapper::exportData(const std::filesystem::path& filePath, const std::string& token) {
	
	if (currentFileName != filePath.filename().string()) {

		if (!currentFileName.empty()) {
			// write to the currentFileName path
			fileManager.WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName);
			
			// clear the buffer after writing
			writeBuffer.clear();
		}
		currentFileName.assign(filePath.filename().string());
	}

	if (!token.empty()) {
		writeBuffer.push_back(token + " 1");
	}

	// this ensures that we always write to the last file, the current logic will always write every file except the last.
	// On the last file, the currentFileName and filePath that is give will match, so you have to ensure to write the last file
	if (!writeBuffer.empty() && token.empty()) {
		fileManager.WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName);
	}
}

