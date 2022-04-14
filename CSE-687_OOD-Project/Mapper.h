#pragma once
#include <filesystem>
#include "Utility.h"

class Mapper
{
public:
	Mapper();
	void map(const std::filesystem::path&, const std::string& line) const;
	void exportData();
};

