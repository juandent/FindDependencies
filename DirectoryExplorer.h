#pragma once
#include <filesystem>

#include "Parser.h"


class DirectoryExplorer
{
public:

	DirectoryExplorer() {}

	void Execute(std::string directory_path)
	{
		using namespace std;
		namespace fs = std::filesystem;

		int count = 0;
		Parser parse{};
		for(auto it = fs::directory_iterator(directory_path); it != fs::directory_iterator{}; ++it)
		{
			if (parse.ProcessFile(*it))
			{
				++count;
			}
		}
		std::cout << std::format("{} module interface files processed", count) << std::endl;
	}

	// bool Dependencies()
	// {
	// 	//bool has_cycle = Module::look_for_ancestors_in_tree();
	// 	return true;  has_cycle;
	// }
};

