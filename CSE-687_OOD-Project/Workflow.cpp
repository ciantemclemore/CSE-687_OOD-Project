#include "Workflow.h"


/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::string& inputDir, const std::string& tempDir, const std::string& outputDir) : inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir) {
}

void Workflow::Init() const {
	std::list<std::filesystem::path> paths = fileManager.GetFilesInDirectory(inputDirectory);
	Mapper mapper(tempDirectory);

	for (const std::filesystem::path& path : paths) {
		std::list<std::string> fileLines = fileManager.GetFileLines(path);

		for (const std::string& line : fileLines) {
			mapper.map(path, line);
		}
	}
}
