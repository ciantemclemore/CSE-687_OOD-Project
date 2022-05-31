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

		// get the files from the provided "input" directory so that they can be mapped
		std::vector<std::filesystem::path> inputFiles = Utilities::GetFilesInDirectory(inputDirectory);

		// this function will split keys into "N" buckets or "N" mappers. There will be a reducer for each mapper
		// there is a 3rd optional argument to the MapperPartition that allows user to specify the number of threads. It is 5 by default
		// NumOfMapper threads can be configurable via command line in the future
		int numReducerThreads = MapperPartition(inputFiles, Map);

		// Get the intermediate files after mapping
		std::vector<std::filesystem::path> tempFiles = Utilities::GetFilesInDirectory(tempDirectory);


		// Notify the user that reducing is beginning
		std::cout << "Reducing process is beginning..." << std::endl;

		// this will create the number of reduer threads based on what the mapper partition returned
		ReducerPartition(tempFiles, Reduce, numReducerThreads);

		return true;
	}
	else {
		std::cout << "MapReduce DLL could not be loaded. Please verify the path to the MapReduce DLL file name." << std::endl;
		return false;
	}
}

int Workflow::MapperPartition(const std::vector<std::filesystem::path>& inputFiles, FuncMap map, const int numMapperThreads) const {
	
	StartTasks(inputFiles, numMapperThreads, map, nullptr);
	return numMapperThreads;
}

void Workflow::ReducerPartition(const std::vector<std::filesystem::path>& tempFiles, FuncReduce reduce, int numOfReducerThreads) const {
	StartTasks(tempFiles, numOfReducerThreads, nullptr, reduce);
}

void Workflow::StartTasks(const std::vector<std::filesystem::path>& files, int numThreads, FuncMap map, FuncReduce reduce) const {
	// bucket the files based on num of threads
	std::vector<std::thread> threads;

	// calculation to determine how many jobs will be given to each thread
	auto jobsPerThread = (files.size() / numThreads) + (files.size() % numThreads != 0);

	for (int i = 0; i < numThreads; i++) {
		std::vector<std::filesystem::path> threadFiles;

		for (auto j = jobsPerThread * i; j < jobsPerThread * i + jobsPerThread; j++) {

			if (j >= files.size())
			{
				break;
			}

			if (map != nullptr) {

				std::cout << "Mapping " << files[j].filename() << std::endl;
			}
			else {
				std::cout << "Reducing " << files[j].filename() << std::endl;
			}

			threadFiles.push_back(files[j]);
		}
		// Pass the list of files for the map function (can either send vector of files, or make another 'for-loop' to go through each file in each vector
		if (map != nullptr) {
			threads.push_back(std::thread(map, threadFiles, tempDirectory));
		}
		else {
			threads.push_back(std::thread(reduce, threadFiles, files.size(), outputDirectory));
		}
	}

	// wait until each mapperThread finishes processing their files
	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}
