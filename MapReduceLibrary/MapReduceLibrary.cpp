#include "pch.h"
#include "framework.h"
#include "MapperLibrary.h"
#include "Utilities.h"

__declspec(dllexport) void Map(const std::filesystem::path& filePath, const std::string& line, const std::filesystem::path& tempOutputPath) {
	auto tokens = Utilities::SplitAndClean(line);
	ExportData(filePath, tokens, tempOutputPath);
}

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
