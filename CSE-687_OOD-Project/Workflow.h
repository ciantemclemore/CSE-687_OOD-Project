#pragma once
#include <iostream>
#include "FileManagement.h"

/// <summary>
/// The workflow class will tie together the use of the mapper and reducer classes.
/// This class most likely will contain functions that start the execution of the other components.
/// Ex. The workflow class can create a reference to a mapper and reducer class in its constructor. 
/// Then, the workflow class can create a function that kicks off mapping...then reducing.
/// </summary>
class Workflow
{
private:
	std::string inputDirectory;
	std::string outputDirectory;
	std::string tempDirectory;
	FileManagement fileManager;
public:
	Workflow(const std::string& inputDir, const std::string& outputDir, const std::string& tempDir);
	void Init() const;
};

