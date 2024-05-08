#pragma once

#include "modules/resourcemanager.hpp"

class CResourceManager : public IResourceManager
{
public:
	CResourceManager();
	~CResourceManager() override;

	virtual void PostInit() {};
	virtual void PreDeinit() {};
	virtual void Update() {};

	virtual TSResult PrecacheResource(const std::string& path, ResourceLoadFlag load_flag);
	virtual TSResult LoadResourceInternal(const std::string& path, ResourceLoadFlag load_flag, IResource*& resource, ResourceLoadOptions* options);

	virtual void IncrementRefResource(IResource* resource);
	virtual void DecrementRefResource(IResource* resource);

	virtual TSResult UnloadResource(IResource* resource);

	virtual void RegisterResourceFactory(IResourceFactory* factory);

	virtual std::map<std::string, IResource*>& GetLoadedResources();

private:
	IResourceFactory* GetFactory(IFileStream* stream);

	std::map<std::string, IResource*> resources;
	std::map<ResourceType, IResourceFactory*> factories;
	std::map<ResourceHeader, ResourceType> type_map;
};

extern CResourceManager* g_ResourceManager;