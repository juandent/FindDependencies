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

    TopologicalSort sorter;


    for (auto& mod : Module::all_modules)
    {
        // is mod in Vertex::all_vertices
        auto ret = Vertex::find(mod);
        assert(ret.get() != nullptr);
    }

    sorter.Execute();
    sorter.PrintOrderedVertices();
    sorter.Verify();

}

