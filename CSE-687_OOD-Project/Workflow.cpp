#include "Workflow.h"

/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::filesystem::path& inputDir, const std::filesystem::path& tempDir, const std::filesystem::path& outputDir, const wchar_t* dllPath)
	: inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir), mapReduceDllPath(dllPath) {
}

bool Workflow::Init() const {

	HINSTANCE mapReduceLibraryHandle;
	FuncMap Map;
	FuncReduce Reduce;

	mapReduceLibraryHandle = LoadLibraryEx(mapReduceDllPath, nullptr, NULL);

	if (mapReduceLibraryHandle != nullptr) {
		
		Map = (FuncMap)GetProcAddress(mapReduceLibraryHandle, "Map");
		Reduce = (FuncReduce)GetProcAddress(mapReduceLibraryHandle, "Reduce");

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

		// Get the intermediate files after mapping and begin sorting/aggregation
		Sorter sorter;
		std::vector<std::filesystem::path> tempFiles = Utilities::GetFilesInDirectory(tempDirectory);

		// Notify the user that sorting and aggregating is taking place
		std::cout << "Sorting and Aggregating all intermediate files..." << std::endl;

		std::map<std::string, std::vector<int>, std::less<>> sortedContainer = sorter.sortAndAggregate(tempFiles);


		// Notify the user that reducing is beginning
		std::cout << "Reducing process is beginning..." << std::endl;

		// Call reduce on each key
		for (const auto& [key, value] : sortedContainer) {
			std::cout << "Reducing " << "'" << key << "' key" << std::endl;
			Reduce(key, value, outputDirectory);
		}
		return true;
	}
	else {
		std::cout << "MapReduce DLL could not be loaded. Please verify the path to the MapReduce DLL file name." << std::endl;
		return false;
	}
}
