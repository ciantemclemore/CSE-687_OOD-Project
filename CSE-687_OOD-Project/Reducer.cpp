#include "Reducer.h"

/// <summary>
/// Constructor
/// </summary>
Reducer::Reducer(const std::string& outputFilePath) : outputFilePath(outputFilePath) {
}

/// <summary>
/// The reduce function will take two parameters. A string key and an iterator of integers.
/// The function will sum all the values in the iterator and then call export.
/// </summary>
void Reducer::Reduce(const std::string& key, const std::vector<int>& iterations) {

	// Sum up the iterations
	int totalCount = 0;
	for (int i = 0; i < iterations.size(); i++) {
		totalCount += iterations[i];
	}
	ExportData(key, totalCount);
}

/// <summary>
/// The export function will take two parameters. A string key and the resulting reduced data from
/// the reduce function. The function will write the result to the output directory. This function should
/// not directly deal with an File IO.
/// </summary>
void Reducer::ExportData(const std::string& key, int reducedData) { //reduced data
	std::list<std::string> buffer = std::list <std::string> ();
	std::string content = key + " " + std::to_string(reducedData);
	buffer.push_back(content);
	FileManagement::WriteBufferToFile(buffer, outputFilePath + "\\" + "finalOutput.txt", std::ios::app);
}
