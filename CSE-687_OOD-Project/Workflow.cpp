#include "Workflow.h"


/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::string& inputDir, const std::string& tempDir, const std::string& outputDir) : inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir) {
}

void Workflow::Init() const {

	// Map the files so that they can be sorted and reduced later
	std::list<std::filesystem::path> paths = fileManager.GetFilesInDirectory(inputDirectory);
	Mapper mapper(tempDirectory);

	for (const std::filesystem::path& path : paths) {
		std::list<std::string> fileLines = fileManager.GetFileLines(path);

		for (const std::string& line : fileLines) {
			mapper.Map(path, line);
		}
	}

	// Get the temporary files after mapping for sorting and aggregation
	std::list<std::filesystem::path> temps = fileManager.GetFilesInDirectory(tempDirectory);
	std::map<std::string, std::vector<int>> sortedContainer = std::map<std::string, std::vector<int>>();
	Reducer reducer(outputDirectory);

	// Aggregating and sorting the keys
	for (const std::filesystem::path& temp : temps) {
		std::list<std::string> keys = fileManager.GetFileLines(temp);

		//how to fit this inside utility header
		for (const std::string& key : keys) {
			
			// get all the tokens in the line
			std::list<std::string> tokens = Utility::SplitAndClean(key);

			// get the first token, which will be the key
			std::string sortKey = tokens.front();

			if (sortedContainer.find(sortKey) == sortedContainer.end()) { // there is such an element
				sortedContainer.insert(std::pair<std::string, std::vector<int>>(sortKey, std::vector<int>(1, 1)));
			}
			else { //such an element does not exist, create new instance
				sortedContainer[sortKey].push_back(1);
			}
		}
	}

	// Call reduce on each key
	for (const auto& sortedPair: sortedContainer) {
		reducer.Reduce(sortedPair.first, sortedPair.second);
	}
}
