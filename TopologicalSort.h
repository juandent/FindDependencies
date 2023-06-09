#pragma once
#include <cassert>
#include <list>

#include "Module.h"

using namespace std;

struct Vertex : public std::enable_shared_from_this<Vertex>
{
	using time_t = unsigned;
	using module_t = std::shared_ptr<Module>;
	using self = std::shared_ptr<Vertex>;
	enum Color {white, gray, black};
	module_t module;
	Color color = Color::white;
	module_t pi = nullptr;
	time_t start = 0;
	time_t finish =0;
	std::vector<std::shared_ptr<Vertex>> dependencies;
	Vertex() = default;

	explicit Vertex(const module_t& module)
		: module(module)	
	{
	}

	std::shared_ptr<Vertex> getptr()
	{
		return shared_from_this();
	}
	bool operator==(const std::shared_ptr<Vertex>& rhs) const noexcept
	{
		return this->module == rhs->module;
	}
	inline static unsigned time = 0;
	inline static std::list<std::shared_ptr<Vertex>> ordered_vertices;
	inline static std::vector<std::shared_ptr<Vertex>> all_vertices;
	inline static std::vector<std::shared_ptr<Vertex>> root_vertices;
	static std::shared_ptr<Vertex> find(module_t mod)
	{
		auto ret = std::find_if(all_vertices.begin(), all_vertices.end(), [&mod](self vertex)
			{
				return vertex->module == mod;
			});
		if(ret != all_vertices.end())
		{
			return *ret;
		}
		return nullptr;
	}
	static bool is_root(const std::shared_ptr<Vertex>& vertex)
	{
		auto ret = std::find_if(root_vertices.begin(), root_vertices.end(), [&vertex](self element)
			{
				return vertex->module == element->module;
			});
		return ret != root_vertices.end();
	}
	static std::shared_ptr<Vertex> find_or_create(module_t mod, ModuleType type)
	{
		auto vertex = Vertex::find(mod);
		if ( ! vertex )
		{
			vertex = std::shared_ptr<Vertex>(new Vertex(mod));
			all_vertices.push_back(vertex);
		}
		if (type == ModuleType::Root)
		{
			if (! Vertex::is_root(vertex))
			{
				root_vertices.push_back(vertex);
			}
		}
		return vertex;
	}

};




class TopologicalSort
{
	// void DFS_Visit(Vertex::self u)
	// {
	// 	auto uname = u->module->name;
	// 	auto color = u->color;
	//
	// 	u->color = Vertex::gray;
	// 	u->start = ++Vertex::time;
	// 	std::cout << std::format("Gray {0} start {1} has {2} dependencies", u->module->name, u->start, u->dependencies.size()) << std::endl;
	// 	for(auto& v : u->dependencies)
	// 	{
	// 		auto vname = v->module->name;
	// 		if(v->color == Vertex::white)
	// 		{
	// 			v->pi = u->module;
	// 			DFS_Visit(v);
	// 		}
	// 	}
	//
	// 	u->color = Vertex::black;
	// 	u->finish = ++Vertex::time;
	// 	std::cout << std::format("Black {0} start {1} has {2} dependencies end {3}", u->module->name, u->start, u->dependencies.size(), u->finish) << std::endl;
	// 	Vertex::ordered_vertices.insert(Vertex::ordered_vertices.begin(), u);
	// }
	std::shared_ptr<Vertex> find(std::shared_ptr<Vertex> vertex)
	{
		auto ret = std::find_if(Vertex::all_vertices.begin(), Vertex::all_vertices.end(), [&vertex](auto& ver)
			{
				return vertex->module->name == ver->module->name;
			});
		if( ret != Vertex::all_vertices.end())
		{
			return *ret;
		}
		return nullptr;
	}
	void print(std::shared_ptr<Vertex> v, int depth)
	{
		using namespace std;
		auto tab = print_tab(depth);

		cout << tab << v->module->name << " deps " << v->dependencies.size() << " is_root " << Vertex::is_root(v) << endl;
		for (bool first = true; auto& u : v->dependencies)
		{
			if (first) {
				first = false;
				++depth;
			}
			cout << " " << u->module->name << endl;
			auto ret = find(u);
			if( ret)
			{
				print(ret, depth);
			}
		}
	}

public:
	TopologicalSort()
	{
		for( auto& mod : Module::all_modules)
		{
			auto v = Vertex::find_or_create(mod, Module::is_root(mod) ? ModuleType::Root : ModuleType::Child);
			auto name = v->module->name;
			for (auto& dep : mod->dependencies)
			{
				// already exists in vertices?
				v->dependencies.push_back(Vertex::find_or_create(dep, ModuleType::Child));
			}
			auto equal = v->dependencies.size() == mod->dependencies.size();
		}

	}
	bool in_ordered_vertices(const std::shared_ptr<Vertex>& vertex)
	{
		auto ret = std::find_if(Vertex::ordered_vertices.begin(), Vertex::ordered_vertices.end(), [&vertex](auto& existing_vertex)
			{
				return vertex == existing_vertex;
			});
		return ret != Vertex::ordered_vertices.end();
	}

	void Follow(const std::shared_ptr<Vertex>& vertex)
	{
		// std::cout << "Encontrada \t" << vertex->module->name << std::endl;
		auto deps = vertex->dependencies;

		for(auto& dep : deps)    // B,C
		{
			// std::cout << "Dependency \t" << dep->module->name << std::endl;
			Follow(dep);
		}
		if (!in_ordered_vertices(vertex))
		{
			// std::cout << "Almacenada \t" << vertex->module->name << std::endl;
			Vertex::ordered_vertices.push_back( vertex);
		}
	}


	void Execute()
	{
		cout << "Sorting!" << endl;
		for( auto& vertex : Vertex::root_vertices)
		{
			// cout << "Working " << vertex->module->name << endl;

			// vertex == A
			Follow(vertex);
		}
	}

	void PrintOrderedVertices()
	{
		for(auto& vertex : Vertex::ordered_vertices)
		{
			cout << " Almacenada: " << vertex->module->name << endl;
		}
	}

	auto get_position(const std::shared_ptr<Vertex>& vertex )
	{
		auto ret = std::find(Vertex::ordered_vertices.begin(), Vertex::ordered_vertices.end(), vertex);
		// auto dist = std::distance(ret, Vertex::ordered_vertices.begin());
		// return dist;
		return ret;
	}
	auto print_position(const std::shared_ptr<Vertex>& vertex, const std::string& msg)
	{
		auto str = std::format("{0:4} : {1}", vertex->module->name, msg);
		cout << str << endl;
		return get_position(vertex);
	}
	void Verify()
	{
		for (auto& vertice : Vertex::ordered_vertices)
		{
			auto root_pos = get_position(vertice); // print_position(vertice, "root");
			for (auto& deps : vertice->dependencies)
			{
				auto deps_pos = get_position(deps);
				auto dist = std::distance(root_pos, deps_pos);
				auto str = std::format("{0:4} root: '{1:15}' dep: '{2:15}'", dist, vertice->module->name, deps->module->name);
				cout << str << endl; 
				assert(dist >= 0);
			}
		}

	}
};

