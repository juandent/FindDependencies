#pragma once



#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <format>

#include "printer.h"

enum class ModuleType
{
	Root,
	Child
};


class Module :  public std::enable_shared_from_this<Module>
{

	std::vector<std::shared_ptr<Module>> duplicate_dependencies;
	inline static std::vector<std::shared_ptr<Module>> module_seen;
public:
	std::vector<std::shared_ptr<Module>> dependencies;
	inline static std::vector<std::shared_ptr<Module>> all_roots;
	inline static std::vector<std::shared_ptr<Module>> all_modules;


	// look for ancestor in the trees
	// from all_modules, check the first module with traversing the whole tree
	// static bool look_for_ancestors_in_tree( )
	// {
	// 	module_seen.clear();
	// 	for (auto& mod : all_modules)
	// 	{
	// 		if (find_module_in_tree(mod, mod)) return true;
	// 	}
	// 	return false;
	// }
	static void print(std::shared_ptr<Module> ancestor, std::shared_ptr<Module> mod, std::shared_ptr<Module> root)
	{
		std::cout << "Ancestor '" << ancestor->name << "'  module '" << mod->name << "'  root '" << root->name << "'" << std::endl;
	}
	// static bool find_module_in_tree(std::shared_ptr<Module> ancestor, std::shared_ptr<Module> tree_root)
	// {
	// 	for (auto& dep : tree_root->dependencies)
	// 	{
	// 		print(ancestor, dep, tree_root);
	// 		if (dep == ancestor)
	// 		{
	// 			std::cout << "Dep " + dep->name + "  root " + tree_root->name << std::endl;
	// 			return true;
	// 		}
	// 		if (find_module_in_tree(ancestor, dep) == true)	return true;
	// 	}
	// 	return false;
	// }
	bool operator==(std::shared_ptr<Module> another)
	{
		return another->name == this->name;
	}

	std::shared_ptr<Module> getptr()
	{
		return shared_from_this();
	}
private:
	// ensure name is a Module in all_modules
	static std::shared_ptr<Module> ensure_module(std::string name)
	{
		auto ret = std::find_if(all_modules.begin(), all_modules.end(),
			[&name](std::shared_ptr<Module> potential_module)
			{
				return potential_module->name == name;
			});
		if (ret == all_modules.end())
		{
			auto m = std::shared_ptr<Module>(new Module{ name });
			all_modules.push_back(m);
			return m;
		}
		else
		{
			return *ret;
		}
	}

public:
	static bool is_root(const std::shared_ptr<Module>& potential_root)
	{
		auto ret = std::find_if(all_roots.begin(), all_roots.end(), [&potential_root](std::shared_ptr<Module>& root)
			{
				return root->name == potential_root->name;
			});
		return ret != all_roots.end();
	}

	static std::shared_ptr<Module> find_or_create(std::string name, ModuleType type )
	{
		auto m = ensure_module(name);

		if( type== ModuleType::Root)
		{
			bool is_a_root = is_root(m);

			if( ! is_a_root ) //not found
			{
				all_roots.push_back(m);
			}
		}
		return m;
	}
	void addDependency(std::string name)
	{
		auto module = Module::find_or_create(name, ModuleType::Child);
		bool exists = exists_already(module);
		if (!exists)
		{
			dependencies.push_back(module);
		}
		else
		{
			duplicate_dependencies.push_back(module);
		}
	}

	void mark_processed()
	{
		marked_processed = true;
	}

	void print(int depth=0)
	{
		using namespace std;
		auto tab = print_tab(depth);
		cout << boolalpha;
		cout << tab << name << endl;					//  << " is root " << is_root(this->getptr()) << endl;
		if (marked_processed && depth >0 )	return;
		++depth;
		ranges::for_each(dependencies, [&depth](const auto& module)
			{
				auto name = module->name;
				if (module->name == "alias")
				{
					int i = 0;
				};
				module->print(depth);
			});
	}
	const std::string name;
private:

	bool marked_processed = false;

	Module(std::string n) : name{n} {}

	// in dependencies
	bool exists_already(std::shared_ptr<Module> candidate)
	{
		auto ret = std::find_if(dependencies.begin(), dependencies.end(), 
			[&candidate](const std::shared_ptr<Module>& potential_dependency)
			{
				return potential_dependency == candidate;
			});
		return ret != dependencies.end();
	}

};

