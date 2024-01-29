#pragma once
#include "modules/pluginmanager.hpp"

#include <map>
#include <vector>

class CPluginManager : public IPluginManager
{
public:
	CPluginManager();
	~CPluginManager() override;

	virtual void PostInit() { };
	virtual void Update() { };
	virtual void PreDeinit();

	virtual IPluginDescription& LoadPlugin(const std::string& path);
	virtual void UnloadPlugin(const std::string& name);

	virtual IPlugin* CreatePlugin(const std::string& name, TSResult& result);

	virtual void DeletePlugin(IPlugin* plugin);

	virtual TSResult EnablePlugin(IPlugin* plugin);
	virtual void DisablePlugin(IPlugin* plugin);

private:
	std::map<std::string, IPluginDescription> loaded_plugins;

	std::vector<IPlugin*> plugins;
};

extern CPluginManager* g_PluginManager;