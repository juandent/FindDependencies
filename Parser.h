#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <exception>

#include "Module.h"

class Parser
{
	std::ifstream my_input;
	std::string find_module_name(const std::string& line, size_t offset_to_import)
	{
		auto rest = line.substr(offset_to_import + 7);
		int i = 0;
		for( i=0; i < rest.size(); ++i)
		{
			char caracter = rest[i];
			if (caracter != ' ' && caracter != '\t')
				break;
		}
		rest = rest.substr(i);

		auto end = rest.find_first_of("\t ;");
		auto module_name = rest.substr(0, end);
		return module_name;
	}
public:

	Parser()
	{}

	bool ProcessFile(std::filesystem::path file)
	{
		using namespace std;
		namespace fs = std::filesystem;

		if( file.extension().string() != ".ixx" )
		{
			return false;
		}
		my_input.open(file.string(), std::ios::in );
		if(!my_input.good())
		{
			string str = format("Could not open {}", file.string());
			throw exception(str.c_str());
		}

		auto parent = Module::find_or_create(file.stem().string(), ModuleType::Root );
		string line;
		do
		{
			getline(my_input, line);
			auto comment_start = line.find("//");
			auto start = line.find("import ");
			if(comment_start != string::npos)
			{
				if(comment_start < start || start == string::npos)
				{
					continue;
				}
			}
			if (start != string::npos)
			{
				auto module_name = this->find_module_name(line, start);
				parent->addDependency(module_name);
			}
			
		} while (my_input.good());
		// mark parent as processed to prevent cycles in printing
		parent->mark_processed();
		parent->print();
		my_input.close();
		return true;
	}
};

