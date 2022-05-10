#pragma once
#include "string"
#include "vector"
#include "filesystem"
#include "sstream"
#include "regex"
#include "fstream"
#include <iostream>

class Utilities
{
public:
	// Splits a string entry by space and selected special characters
	static std::vector<std::string> SplitAndClean(const std::string& entry);
	
	// Removes special characters from a string entry
	static std::string RemoveSpecialCharacters(const std::string& entry);
	
	// Makes every character of a string lowercase
	static void ToLower(std::string& entry);
	
	// Returns all file paths in a specified directory
	static std::vector<std::filesystem::path> GetFilesInDirectory(const std::filesystem::path& directory);
	
	// Breaks down every line of a text file and returns them as individual string entry's
	static std::vector<std::string> GetFileLines(const std::filesystem::path& path);
	
	// Writes buffer data to a specified file
	static void WriteBufferToFile(const std::list<std::string>& buffer, const std::filesystem::path& path, std::ios_base::openmode mode);
};

