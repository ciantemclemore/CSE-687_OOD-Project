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
			mapper.Map(path, line);
		}
	}

	std::list<std::filesystem::path> temps = fileManager.GetFilesInDirectory(tempDirectory);
	Reducer reducer(outputDirectory);
	std::map<std::string, std::list<int>> sortedContainer;

	for (const std::filesystem::path& temp : temps) {
		std::list<std::string> keys = fileManager.GetFileLines(temp);
		//how to fit this inside utility header
		for (const std::string& key : keys) {
			if (sortedContainer.find(key) != sortedContainer.end()) { // there is such an element
				sortedContainer.insert(std::pair<std::string, int>(key, 1));
			}
			else { //such an element does not exist, create new instance
				sortedContainer[key].push_back(1);
			}
			//std::list<int> iterations = Utility::Sort(key);

		}
	}
	for (const auto& sortedPair : sortedContainer) {
		reducer.Reduce(sortedPair.first, sortedPair.second);
	}
}
