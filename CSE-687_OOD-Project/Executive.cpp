// Executive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Workflow.h"
#include "filesystem"
#include "fstream"
#include "string"

/// <summary>
/// The main function will accept 3 command line arguments.
/// 1. Directory that holds the input files to MapReduce
/// 2. Diectory to hold output files
/// 3. Temporary directory to hold intermediate output files
/// </summary>
/// <param name="argc">The number of arguments passed to the command line</param>
/// <param name="argv">An array of char pointers that contain all the arguments</param>
/// <returns></returns>
int main(int argc, char* argv[])
{
    // The program should accept 3 inputs (plus the included default argument, i.e Name of the program)
    if (argc < 4) {
        std::cout << "Missing input arguments, try again!" << std::endl;
        exit(0);
    }
    
    const char* directory = "../../CSE-687_OOD-Project/CSE-687_OOD-Project/testfiles";

    // Give the input, output, and temp directories to the workflow component.
    Workflow workflow(directory, argv[2], argv[3]);
    workflow.Init();
    
    std::cin.get();
}
