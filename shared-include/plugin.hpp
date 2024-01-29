#pragma once
#include "engine.hpp"
#include "global.hpp"
#include "Windows.h"

enum class IPluginStage
{
	REGISTER_COMPONENTS,
	POST_REGISTER_COMPONENTS,

	LOAD_GAME,
	POST_LOAD_GAME,

	LOAD_WORLD,
	POST_LOAD_WORLD,

	SHUTDOWN,
};

class IPlugin;

typedef IPlugin* (*PROC_PluginCreate)(IEngine* engine);
typedef void (*PROC_PluginDelete)(IPlugin* plugin);

typedef struct IPluginDescription_t
{
	char* name;
	TSVersion version;
	PROC_PluginCreate pluginCreate;
	PROC_PluginDelete pluginDelete;

	HMODULE dll;

	IPluginDescription_t()
	{
		name = NULL;
		version = TSVersion(0, 0, 0);

		pluginCreate = NULL;
		pluginDelete = NULL;

		dll = NULL;
	}

	IPluginDescription_t(char* name, TSVersion version) : IPluginDescription_t()
	{
		this->name = name;
		this->version = version;
	}

	IPluginDescription_t(char* name, TSVersion version, PROC_PluginCreate pluginCreate, PROC_PluginDelete pluginDelete) : IPluginDescription_t(name, version)
	{
		this->pluginCreate = pluginCreate;
		this->pluginDelete = pluginDelete;
	}
} IPluginDescription;

class IPlugin
{
public:
	virtual ~IPlugin() = default;

	virtual TSResult Initialize(IEngine* engine) = 0;

	virtual void StageHandler(IPluginStage stage) = 0;

	virtual TSResult Enable() = 0;
	virtual void Disable() = 0;

	virtual bool GetEnabled() { return enabled; };

	IPluginDescription* GetDesc() { return desc; };
	void SetDesc(IPluginDescription* desc) { this->desc = desc; };
private:
	bool enabled;
	IPluginDescription* desc;
};

typedef IPluginDescription(*PROC_PluginDescription)(void);

#define DEFINE_PLUGIN(name, ver_maj, ver_min, ver_patch) \
IPlugin* CreatePlugin(IEngine* engine); \
ENGINE_API ENGINE_EXPORT IPluginDescription GetPluginDescription() \
{ \
	return IPluginDescription(name, TSVersion(ver_maj, ver_min, ver_patch), CreatePlugin, NULL); \
} \
IPlugin* CreatePlugin(IEngine* engine)