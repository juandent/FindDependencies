#include <cassert>

#include "TopologicalSort.h"

void TestInitial()
{
	TopologicalSort sorter;


	for( auto& mod : Module::all_modules)
	{
		// is mod in Vertex::all_vertices
		auto ret = Vertex::find(mod);
		assert(ret.get() != nullptr);
	}

	sorter.Execute();
	sorter.PrintOrderedVertices();
	sorter.Verify();
}
