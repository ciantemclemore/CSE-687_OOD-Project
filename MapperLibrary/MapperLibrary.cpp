#include "pch.h"
#include "framework.h"
#include "MapperLibrary.h"
#include "Utilities.h"

// dll variables
std::list<std::string> writeBuffer;
std::string currentFileName;
const std::string outputFilePath;
size_t totalBufferCount;

MAPPERLIBRARY_EXPORTS void Map(const std::filesystem::path& filePath, const std::string& line) {
	auto tokens = Utilities::SplitAndClean(line);
	totalBufferCount = tokens.size();

	for (const std::string& token : tokens) {
		ExportData(filePath, token);
	}
}

MAPPERLIBRARY_EXPORTS void ExportData(const std::filesystem::path& filePath, const std::string& token) {
	if (!token.empty()) {
		writeBuffer.push_back(token + " 1");
		totalBufferCount--;
	}

	if (currentFileName != filePath.filename().string() || totalBufferCount == 0) {

		if (!currentFileName.empty() && totalBufferCount == 0) {

			// write the buffer to the file
			if (std::filesystem::exists(outputFilePath + "\\" + currentFileName)) {
				Utilities::WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName, std::ios::app);
			}
			else {
				Utilities::WriteBufferToFile(writeBuffer, outputFilePath + "\\" + currentFileName, std::ios::out);
			}

			// clear the buffer after writing
			writeBuffer.clear();
		}
		currentFileName.assign(filePath.filename().string());
	}
}
