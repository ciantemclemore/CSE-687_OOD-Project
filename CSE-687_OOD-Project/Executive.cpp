// Executive.cpp : This file contains the 'main' function. Program execution begins and ends there.
#pragma once 
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "Workflow.h"
#include "filesystem"
#include "fstream"
#include "string"
#include "Utilities.h"

/// <summary>
/// The main function will accept  command line arguments.
/// 1. Directory that holds the input files to MapReduce
/// 2. Diectory to hold output files
/// 3. Temporary directory to hold intermediate output files
/// 4. Path to a MapReduce DLL
/// </summary>
/// <param name="argc">The number of arguments passed to the command line</param>
/// <param name="argv">An array of char pointers that contain all the arguments</param>
/// <returns></returns>
int main(int argc, char* argv[])
{
    // The program should accept at least 2 inputs (i.e Name of the program & directory for input files)
    // If their are more than 2 arguments, the user may have provided an intermediate and output directory
    if (argc < 2) {
        std::cout << "Missing input files directory, try again!" << std::endl;
        exit(0);
    }

    // save the input directory as a path (this is done to follow our "path" standard)
    std::filesystem::path inputDir = argv[1];

    // If the user doesn't supply a temp and output directory, we will create it for them
    std::filesystem::path tempDir =  argc > 2  ? argv[2] : std::filesystem::current_path().string() + "\\tempfiles";
    std::filesystem::path outputDir = argc > 3 ? argv[3] : std::filesystem::current_path().string() + "\\outputfiles";
    auto mapReduceDllName = argc > 4 ? std::filesystem::path(argv[4]) : L""; // convert the char* to a path so that we can successfully convert to a wchar_t*
    const wchar_t* mapReduceLibraryName = !(mapReduceDllName.empty()) ? mapReduceDllName.c_str() : L"MapReduceLibrary.dll";

    // create the directories for the temp/intermidate and final output files
    std::filesystem::create_directory(tempDir);
    std::filesystem::create_directory(outputDir);

    if (std::filesystem::is_directory(inputDir) && std::filesystem::is_directory(tempDir) && std::filesystem::is_directory(outputDir)) {
        // Give the input, output, and temp directories to the workflow component.
        Workflow workflow(inputDir, tempDir, outputDir, mapReduceLibraryName);

        // Update the user that the process is starting 
        std::cout << "Beginning Processing..." << std::endl;
        
        // begin the work for creating the server processes and connecting client sockets
        workflow.Init();

        // temp file cleanup
        auto files = Utilities::GetFilesInDirectory(tempDir);

        if (files.size() > 0) {
            for (const auto& file : files) {
                if (file.filename().string() != "readme.txt") {
                    remove(file);
                }
            }
            // remove the temp directory
            std::filesystem::remove(tempDir);
        }
        
        std::cout << "All temp files cleaned up in " << tempDir << std::endl;
        std::cout << "Final Output File at: " << outputDir << std::endl;
    }
    else {
        std::cout << "One or more of the provided file directories don't exist. Retry!";
        exit(0);
    }
}
