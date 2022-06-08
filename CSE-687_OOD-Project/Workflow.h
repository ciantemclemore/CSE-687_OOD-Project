#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "Utilities.h"
#include "Sorter.h"
#include "map"
#include "filesystem"
#include <wtypes.h>
#include "thread"
#include "cmath"

using FuncMap = void (*)(const std::vector<std::filesystem::path>& filePaths, const std::filesystem::path& tempOutputPath);
using FuncReduce = void (*)(const std::vector<std::filesystem::path>& filePaths, int totalFileCount, const std::filesystem::path& outputFilePath);

/// <summary>
/// The workflow class will tie together the use of the mapper and reducer classes.
/// This class most likely will contain functions that start the execution of the other components.
/// Ex. The workflow class can create a reference to a mapper and reducer class in its constructor. 
/// Then, the workflow class can create a function that kicks off mapping...then reducing.
/// </summary>
class Workflow
{
private:
	const std::filesystem::path inputDirectory;
	const std::filesystem::path tempDirectory;
	const std::filesystem::path outputDirectory;
	const wchar_t* mapReduceDllPath;
	const unsigned short int BASEPORT = 20000;
public:
	Workflow(const std::filesystem::path& inputDir, const std::filesystem::path& tempDir, const std::filesystem::path& outputDir, const wchar_t* dllPath);
	bool Init() const;
	int MapperPartition(const std::vector<std::filesystem::path>& inputFiles, FuncMap map, int numMapperThreads = 5) const;
	void ReducerPartition(const std::vector<std::filesystem::path>& tempFiles, FuncReduce reduce, int numOfReducerThreads) const;
	void StartTasks(const std::vector<std::filesystem::path>& files, int numThreads, FuncMap map = nullptr, FuncReduce reduce = nullptr) const;
	std::vector<PROCESS_INFORMATION> CreateServerProcesses() const;
	void ProcessCleanup(const std::vector<PROCESS_INFORMATION>& proc_informations)const;
	void Error(const std::string& message) const;
};

