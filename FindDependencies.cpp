// FindDependencies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <format>

#include "DirectoryExplorer.h"
#include "SortModules.h"
#include "Tests.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

    if (argc == 1)   return -1;

    auto directory = argv[1];

    DirectoryExplorer explorer;
    explorer.Execute(directory);
    //bool has_cycles = explorer.Dependencies();

    // SortModules sorter(Vertex::all_vertices, Module::all_modules);
    // sorter.Sort();

    //std::cout << std::format("{} cycles", has_cycles ? "Has" : "Has no") << std::endl;

    TestInitial();
}

