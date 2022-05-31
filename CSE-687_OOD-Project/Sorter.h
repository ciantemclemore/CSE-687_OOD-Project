#pragma once
#include "Utilities.h"
#include "map"

class Sorter {
public:
	Sorter() = default;
	std::map<std::string, std::vector<int>, std::less<>> sortAndAggregate(const std::vector<std::filesystem::path>& paths) const {

		// map that will hold the sorted and aggregated data
		std::map<std::string, std::vector<int>, std::less<>> sortedContainer;

		for (const std::filesystem::path& path : paths) {

			std::vector<std::string> keys = Utilities::GetFileLines(path);

			for (const std::string& key : keys) {

				// get all the tokens in the line
				std::vector<std::string> tokens = Utilities::Split(key);

				// get the first token, which will be the key
				std::string sortKey = tokens[0];

				// place the token in the map if it doesn't exist.. else - push to the tokens current vector
				if (!sortedContainer.try_emplace(sortKey, 1, 1).second) {
					sortedContainer[sortKey].push_back(1);
				}
			}
		}

		return sortedContainer;
	}
};
