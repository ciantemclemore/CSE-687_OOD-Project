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

		// bucket the files based on num of threads
		std::vector<std::thread> mapperThreads;
		
		// Specify the numbers of threads we want for each key (file) in the mapper process
		// this can be configurable as well via command line
		int totalThreadCount = 5;
		auto jobsPerThread = (inputFiles.size() / totalThreadCount) + (inputFiles.size() % totalThreadCount != 0);
		
		for (int i = 0; i < totalThreadCount; i++) {
			std::vector<std::filesystem::path> files;

			for (auto j = jobsPerThread * i; j < jobsPerThread * i + jobsPerThread; j++) {
				
				if (j >= inputFiles.size()) 
				{
					break;
				}

				std::cout << "Mapping " << inputFiles[j].filename() << std::endl;
				files.push_back(inputFiles[j]);
			}
			// Pass the list of files for the map function (can either send vector of files, or make another 'for-loop' to go through each file in each vector
			mapperThreads.push_back(std::thread(Map, files, tempDirectory));
		}

		// wait until each mapperThread finishes processing their files
		for (auto& thread : mapperThreads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		

		// Get the intermediate files after mapping and begin sorting/aggregation
		//Sorter sorter;
		std::vector<std::filesystem::path> tempFiles = Utilities::GetFilesInDirectory(tempDirectory);

		//// Notify the user that sorting and aggregating is taking place
		//std::cout << "Sorting and Aggregating all intermediate files..." << std::endl;

		//std::map<std::string, std::vector<int>, std::less<>> sortedContainer = sorter.sortAndAggregate(tempFiles);


		// Notify the user that reducing is beginning
		std::cout << "Reducing process is beginning..." << std::endl;

		std::vector<std::thread> reducerThreads;

		for (int i = 0; i < totalThreadCount; i++) {
			std::vector<std::filesystem::path> files;

			for (auto j = jobsPerThread * i; j < jobsPerThread * i + jobsPerThread; j++) {

				if (j >= tempFiles.size())
				{
					break;
				}

				std::cout << "Reducing " << tempFiles[j].filename() << std::endl;
				files.push_back(tempFiles[j]);
			}
			// Pass the list of files for the map function (can either send vector of files, or make another 'for-loop' to go through each file in each vector
			reducerThreads.push_back(std::thread(Reduce, files, tempFiles.size(), outputDirectory));
		}

		// wait until each mapperThread finishes processing their files
		for (auto& thread : reducerThreads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}


		// Call reduce on each key
		/*for (const auto& [key, value] : sortedContainer) {
			std::cout << "Reducing " << "'" << key << "' key" << std::endl;
			Reduce(key, value, outputDirectory);
		}*/
		return true;
	}
	else {
		std::cout << "MapReduce DLL could not be loaded. Please verify the path to the MapReduce DLL file name." << std::endl;
		return false;
	}
}
