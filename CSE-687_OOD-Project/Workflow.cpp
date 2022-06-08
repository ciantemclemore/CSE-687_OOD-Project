#include "Workflow.h"

/// <summary>
/// Constructor to initialize the workflow component
/// </summary>
Workflow::Workflow(const std::filesystem::path& inputDir, const std::filesystem::path& tempDir, const std::filesystem::path& outputDir, const wchar_t* dllPath)
	: inputDirectory(inputDir), tempDirectory(tempDir), outputDirectory(outputDir), mapReduceDllPath(dllPath) {
}

bool Workflow::Init() const {
	
	// create each server process and store the process information for later cleanup
	std::vector<PROCESS_INFORMATION> process_informations = CreateServerProcesses();

	// WSA Startup initialization to use winsocks
	WSAData wsa_data = {};
	if (long wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data); wsa_status != 0) {
		Error("error initializing WSA Startup");
	}

	// server socket collection
	std::vector<SOCKET> server_sockets;

	// first connect each client to the server
	for (unsigned short int i = 0; i < 3; i++) {
		// create a socket to connect to the server
		SOCKET server_socket;
		if ((server_socket = socket(AF_INET, SOCK_STREAM, NULL)) == -1) {
			Error("error opening the socket");
		}

		unsigned short int portNumber = BASEPORT + i;

		// define the server address that the client wants to connect to
		sockaddr_in address;
		inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
		address.sin_family = AF_INET;
		address.sin_port = htons(portNumber);

		// try connecting to the server
		if (int connection_status = connect(server_socket, (struct sockaddr*)&address, sizeof(address)); connection_status == -1) {
			std::cout << WSAGetLastError() << std::endl;
			Error("error connecting to the server socekt");
		}
	}
	


	


	

	// clean up and close all of the process handles
	ProcessCleanup(process_informations);
	
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

std::vector<PROCESS_INFORMATION> Workflow::CreateServerProcesses() const {
	
	std::vector<PROCESS_INFORMATION> process_informations;

	for (int i = 0; i < 3; i++) {
		STARTUPINFO si = {};
		PROCESS_INFORMATION pi = {};

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		int portNumber = BASEPORT + i;
		std::string applicationName = "..\\x64\\Debug\\MapReduce-SocketServer.exe";
		std::string command_arg_string = applicationName.append(" ").append(std::to_string(portNumber));
		wchar_t command_args[60] = {};

		// copy the command line argument string to the LPWSTR
		for (int j = 0; j < command_arg_string.size(); j++)
		{
			command_args[j] = command_arg_string[j];
		}

		if (!CreateProcess(NULL, command_args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			std::cout << "Create Process failed: " << GetLastError() << std::endl;
			exit(1);
		}

		// store the process information to close the handle on the process & thread later
		process_informations.emplace_back(pi);
	}

	return process_informations;
}

void Workflow::ProcessCleanup(const std::vector<PROCESS_INFORMATION>& proc_informations) const {
	// close all the handles and threads for each process
	for(const auto& pi : proc_informations) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void Workflow::Error(const std::string& message) const {
	std::cout << message << std::endl;
	exit(1);
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
