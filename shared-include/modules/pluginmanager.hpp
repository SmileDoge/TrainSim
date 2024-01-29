#pragma once
#include "module.hpp"
#include "plugin.hpp"

class IPluginManager : public IModule
{
public:
	virtual ~IPluginManager() = default;

	virtual IPluginDescription& LoadPlugin(const std::string& path) = 0;
	virtual void UnloadPlugin(const std::string& name) = 0;

	virtual IPlugin* CreatePlugin(const std::string& name, TSResult& result) = 0;

	virtual void DeletePlugin(IPlugin* plugin) = 0;

	virtual TSResult EnablePlugin(IPlugin* plugin) = 0;
	virtual void DisablePlugin(IPlugin* plugin) = 0;
};