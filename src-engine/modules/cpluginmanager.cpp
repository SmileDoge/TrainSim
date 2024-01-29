#include "cpluginmanager.hpp"
#include "cengine.hpp"

#include "clogmodule.hpp"

#include <string>
#include <locale>
#include <codecvt>

CPluginManager* g_PluginManager = NULL;

IPluginDescription null_plugin = IPluginDescription();

CPluginManager::CPluginManager() : loaded_plugins(), plugins()
{
	g_PluginManager = this;
}

void CPluginManager::PreDeinit()
{
	for (auto& plugin : plugins)
	{
		auto desc = plugin->GetDesc();
		if (desc->pluginDelete != NULL)
			plugin->GetDesc()->pluginDelete(plugin);
		else
			delete plugin;
	}

	plugins.clear();
}

CPluginManager::~CPluginManager()
{

}

IPluginDescription& CPluginManager::LoadPlugin(const std::string& path)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	std::wstring wstr = converter.from_bytes(path);

	HMODULE dll = LoadLibrary(wstr.c_str());

	if (dll == NULL)
		return null_plugin;

	PROC_PluginDescription pluginDescription = (PROC_PluginDescription)GetProcAddress(dll, "GetPluginDescription");

	if (pluginDescription == NULL)
		return null_plugin;

	IPluginDescription desc = pluginDescription();

	g_Log->LogDebug("Plugin (%s) Version %d.%d.%d\nPluginCreate_PROC %p\nPluginDelete_PROC %p", desc.name, desc.version.major, desc.version.minor, desc.version.patch, desc.pluginCreate, desc.pluginDelete);

	desc.dll = dll;

	loaded_plugins[desc.name] = desc;

	return loaded_plugins[desc.name];
}

void CPluginManager::UnloadPlugin(const std::string& name)
{
	if (loaded_plugins.find(name) == loaded_plugins.end())
		return;

	auto end = std::remove_if(plugins.begin(), plugins.end(), [&](IPlugin* plugin)
		{
			return plugin->GetDesc()->name == name;
		});

	for (auto start = end; start != plugins.end(); start++)
	{
		auto plugin = *start;

		auto desc = plugin->GetDesc();
		if (desc->pluginDelete != NULL)
			plugin->GetDesc()->pluginDelete(plugin);
		else
			delete plugin;
	}

	plugins.erase(end, plugins.end());

	HMODULE dll = loaded_plugins[name].dll;

	FreeLibrary(dll);

	loaded_plugins.erase(name);
}

IPlugin* CPluginManager::CreatePlugin(const std::string& name, TSResult& result)
{
	if (loaded_plugins.find(name) == loaded_plugins.end())
	{
		result = TS_NOT_FOUND;
		return NULL;
	}

	result = TS_OK;

	auto& desc = loaded_plugins[name];

	IPlugin* plugin = desc.pluginCreate(g_Engine);

	if (plugin == NULL)
	{
		result = TS_INVALID_OPERATION;
		return NULL;
	}

	plugin->SetDesc(&desc);

	plugin->Initialize(g_Engine);

	plugins.push_back(plugin);

	return plugin;
}

void CPluginManager::DeletePlugin(IPlugin* plugin)
{
	if (plugin == NULL) return;

	auto desc = plugin->GetDesc();

	if (desc->pluginDelete != NULL)
		desc->pluginDelete(plugin);
	else
		delete plugin;
}

TSResult CPluginManager::EnablePlugin(IPlugin* plugin)
{
	if (plugin == NULL) return TS_INVALID_ARGS;

	return plugin->Enable();
}

void CPluginManager::DisablePlugin(IPlugin* plugin)
{
	if (plugin == NULL) return;

	plugin->Disable();
}