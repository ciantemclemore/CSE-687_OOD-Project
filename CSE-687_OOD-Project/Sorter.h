#pragma once
#include "FileManagement.h"
#include "Utilities.h"

class Sorter {
public:
	Sorter() = default;
	std::map<std::string, std::vector<int>> sortAndAggregate(std::vector<std::filesystem::path>& paths) {
		
		// map that will hold the sorted and aggregated data
		std::map<std::string, std::vector<int>> sortedContainer;

		for (const std::filesystem::path& path : paths) {

			std::vector<std::string> keys = FileManagement::GetFileLines(path);

			for (const std::string& key : keys) {

				// get all the tokens in the line
				std::vector<std::string> tokens = Utilities::SplitAndClean(key);

				// get the first token, which will be the key
				std::string sortKey = tokens[0];

				// place the token in the map if it doesn't exist.. else - push to the tokens current vector
				if (!sortedContainer.emplace(sortKey, std::vector<int>(1, 1)).second) {
					sortedContainer[sortKey].push_back(1);
				}
			}
		}

		return sortedContainer;
	}
};
