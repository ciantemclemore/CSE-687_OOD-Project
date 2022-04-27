#pragma once
#include "string"
#include "sstream"
#include "list"
#include "map"
#include "regex";

class Utilities {
public:
	Utilities() = default;
	std::vector<std::string> static SplitAndClean(const std::string& entry) {

		// push the string to the stream
		std::stringstream stream(entry);

		// create a list that will store the split tokens
		std::vector<std::string> tokens;

		// generate a regex pattern to remove special the selected special characters
		// words with apostrophe's will be handled separately so we don't split plural words
		std::regex regex_match("[^\\.,:;!? -]+");
		auto words_start = std::sregex_iterator(entry.begin(), entry.end(), regex_match);
		auto words_end = std::sregex_iterator();

		for (std::sregex_iterator word = words_start; word != words_end; ++word) {
			auto x  = (*word).str();
			auto newWord = RemoveSpecialCharacters(x);
			
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

			for (int i = 0; i < temp.size(); i++) {
				if ((temp[i] < 'A' || temp[i] > 'Z') && (temp[i] < 'a' || temp[i] > 'z') && temp[i] != '\0') {
					temp.erase(i, 1);
					i--;
				}
			}
		}

		return temp;
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
