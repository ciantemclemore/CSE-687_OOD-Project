#include "Workflow.h"

/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::string& inputDir, const std::string& tempDir, const std::string& outputDir) : inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir) {
}

void Workflow::Init() const {

	HINSTANCE mapLibraryHandle;
	FuncMap Map;
	const wchar_t* libraryName = L"MapperLibrary";

	mapLibraryHandle = LoadLibraryEx(libraryName, nullptr, NULL);

	if (mapLibraryHandle != nullptr) {
		Map = (FuncMap)GetProcAddress(mapLibraryHandle, "Map");

		// Map the files so that they can be sorted and reduced later
		std::vector<std::filesystem::path> inputFiles = Utilities::GetFilesInDirectory(inputDirectory);

		// notify the user that mapping is beginning
		std::cout << "Mapping process is beginning..." << std::endl;

		for (const std::filesystem::path& file : inputFiles) {
			std::vector<std::string> fileLines = Utilities::GetFileLines(file);
			std::cout << "Mapping " << file.filename() << std::endl;

			for (const std::string& line : fileLines) {

				Map(file, line, tempDirectory);
			}
		}
	}

	// Get the intermediate files after mapping and begin sorting/aggregation
	Sorter sorter;
	std::vector<std::filesystem::path> tempFiles = Utilities::GetFilesInDirectory(tempDirectory);

	// Notify the user that sorting and aggregating is taking place
	std::cout << "Sorting and Aggregating all intermediate files..." << std::endl;

	std::map<std::string, std::vector<int>, std::less<>> sortedContainer = sorter.sortAndAggregate(tempFiles);


	// Notify the user that reducing is beginning
	std::cout << "Reducing process is beginning..." << std::endl;

	// Begin the reducing phase
	Reducer reducer(outputDirectory);

	// Call reduce on each key
	for (const auto& [key, value] : sortedContainer) {
		std::cout << "Reducing " << "'" << key << "' key" << std::endl;
		reducer.Reduce(key, value);
	}
}
