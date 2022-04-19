#pragma once
#include <string>
#include <sstream>
#include <list>
#include <map>

class Utility {
public:
	std::list<std::string> static SplitAndClean(const std::string& entry) {

		// push the string to the stream
		std::stringstream stream(entry);

		// create a list that will store the split tokens
		std::list<std::string> tokens;

		// this string will hold the current token
		std::string word;

		// save all of our tokens
		while (stream >> word) {
			std::string newWord = RemoveSpecialCharacters(word);

			if (!newWord.empty()) {
				ToLower(newWord);
				tokens.push_back(newWord);
			}
		}

		return tokens;
	}
	std::string static RemoveSpecialCharacters(const std::string& entry) {

		std::string temp = entry;

		if (!temp.empty()) {

			for (int i = 0; i < entry.size(); i++) {
				if ((temp[i] < 'A' || temp[i] > 'Z') && (temp[i] < 'a' || temp[i] > 'z') && temp[i] != '\0') {
					temp.erase(i, 1);
					i--;
				}
			}
		}

		return temp;
	}
	std::list<int> Sort(const std::string& entry) {

		std::string key = entry;

		// create a list that will store the the total iterations
		std::list<int> iterations;

		return iterations;

	}
private:
	void static ToLower(std::string& entry) {
		for (char& c : entry) {
			// only want to do characters that are in range of 65-90 (A-Z)
			if (c < 97)
				c = (char)std::tolower(c);
		}
	}
};
