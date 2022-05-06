#pragma once
#include <iostream>
#include "Utilities.h"
#include "Reducer.h"
#include "Sorter.h"
#include "map"
#include <wtypes.h>

using FuncMap = void (*)(const std::filesystem::path& filePath, const std::string& line, const std::filesystem::path& tempOutputPath);

/// <summary>
/// The workflow class will tie together the use of the mapper and reducer classes.
/// This class most likely will contain functions that start the execution of the other components.
/// Ex. The workflow class can create a reference to a mapper and reducer class in its constructor. 
/// Then, the workflow class can create a function that kicks off mapping...then reducing.
/// </summary>
class Workflow
{
private:
	const std::string inputDirectory;
	const std::string tempDirectory;
	const std::string outputDirectory;
public:
	Workflow(const std::string& inputDir, const std::string& tempDir, const std::string& outputDir);
	void Init() const;
};

