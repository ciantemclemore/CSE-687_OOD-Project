#include "../include/Utilities.h"


std::vector<std::string> Utilities::SplitAndClean(const std::string& entry) {

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
		auto x = (*word).str();
		auto newWord = RemoveSpecialCharacters(x);

		if (!newWord.empty()) {
			tokens.push_back(newWord);
		}
	}

	return tokens;
}

std::vector<std::string> Utilities::Split(const std::string& entry) {
	
	// push the string to the stream
	std::stringstream stream(entry);

	// create a list that will store the split tokens
	std::vector<std::string> tokens;

	std::string word;
	while (stream >> word) {
		tokens.push_back(word);
	}

	return tokens;
}

std::string Utilities::RemoveSpecialCharacters(const std::string& entry) {

	std::string temp = entry;

	if (!temp.empty()) {

		int i = 0;
		while (i < temp.size()) 
		{
			if ((temp[i] < 'A' || temp[i] > 'Z') && (temp[i] < 'a' || temp[i] > 'z') && temp[i] != '\0') {
				temp.erase(i, 1);
				i--;
			}
			else {
				// lower case the word
				temp[i] = (char)std::tolower(temp[i]);
			}
			i++;
		}
	}

	return temp;
}

std::vector<std::filesystem::path> Utilities::GetFilesInDirectory(const std::filesystem::path& directory) {

	std::vector<std::filesystem::path> files;

	// Determine if the directory exists or not
	if (!std::filesystem::is_directory(directory)) {
		std::cout << "Directory does not exist, please create directory " << "\"" << directory << "\"" << std::endl;
		exit(0);
	}

	if (!directory.empty()) {
		for (const auto& entry : std::filesystem::directory_iterator(directory)) {
			files.push_back(entry.path());
		}
	}

	return files;
}

std::vector<std::string> Utilities::GetFileLines(const std::filesystem::path& path) {

	std::vector<std::string> fileLines;
	std::fstream file;
	file.open(path, std::ios::in);

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			fileLines.push_back(line);
		}
		file.close();
	}
	else {
		std::cout << "Path to file does not exist, please create a file at path " << path << std::endl;
		exit(0);
	}

	return fileLines;
}

void Utilities::WriteBufferToFile(const std::list<std::string>& buffer, const std::filesystem::path& path, std::ios_base::openmode mode) {

	std::fstream file;
	file.open(path, mode);

	if (file.is_open()) {

		for (const std::string& data : buffer) {
			file << data << std::endl;
		}
		file.close();
	}
}