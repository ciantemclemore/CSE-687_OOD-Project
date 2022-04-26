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
void Mapper::Map(const std::filesystem::path& filePath, const std::string& line) {
	
	auto tokens = Utility::SplitAndClean(line);
	totalBufferCount = tokens.size();

	for (const std::string& token : tokens) {
		ExportData(filePath, token);
	}
}

/// <summary>
/// The export function accept two parameters. A key (filename) and a value (formatted/tokenized line of data).
/// The function will buffer output in memory and periodically write the data out to disk.
/// </summary>
void Mapper::ExportData(const std::filesystem::path& filePath, const std::string& token) {
	
	if (!token.empty()) {
		writeBuffer.push_back(token + " 1");
		totalBufferCount--;
	}

	if (currentFileName != filePath.filename().string() || totalBufferCount == 0) {

		if (!currentFileName.empty() && totalBufferCount == 0) {
			
			// write the buffer to the file
			if (std::filesystem::exists(outputFilePath + "\\" + currentFileName)) {
				fileManager.WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName, std::ios::app);
			}
			else {
				fileManager.WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName, std::ios::out);
			}
			
			// clear the buffer after writing
			writeBuffer.clear();
		}
		currentFileName.assign(filePath.filename().string());
	}
}

