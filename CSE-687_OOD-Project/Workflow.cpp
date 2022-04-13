#include "Workflow.h"


/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::string& inputDir, const std::string& outputDir, const std::string& tempDir) : inputDirectory(inputDir), outputDirectory(outputDir), tempDirectory(tempDir) {
}

void Workflow::Init() const {

	std::list<std::filesystem::path> paths = fileManager.GetFilesInDirectory(inputDirectory);

	for (const std::filesystem::path& path : paths) {
		fileManager.ReadFileByLine(path);
	}
}
