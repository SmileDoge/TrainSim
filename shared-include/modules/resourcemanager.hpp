#pragma once

#include "global.hpp"
#include "module.hpp"
#include "resources/resource.hpp"
#include "modules/filesystem.hpp"

#include <vector>
#include <map>
#include <string>

typedef unsigned int ResourceHeader;

#define GET_RESOURCE_HEADER(header) ( (header[3] << 24) + (header[2] << 16) + (header[1] << 8) + header[0] )

class IResourceFactory
{
public:
	virtual ~IResourceFactory() = default;

	virtual TSResult CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options) = 0;

	virtual ResourceType GetResourceType() = 0;
	virtual ResourceHeader GetHeader() = 0;
};

class IResourceManager : public IModule
{
public:
	virtual ~IResourceManager() = default;

	virtual TSResult PrecacheResource(const std::string& path, ResourceLoadFlag load_flag) = 0;
	virtual TSResult LoadResourceInternal(const std::string& path, ResourceLoadFlag load_flag, IResource*& resource, ResourceLoadOptions* options) = 0;

	virtual void IncrementRefResource(IResource* resource) = 0;
	virtual void DecrementRefResource(IResource* resource) = 0;

	virtual TSResult UnloadResource(IResource* resource) = 0;

	virtual void RegisterResourceFactory(IResourceFactory* factory) = 0;

	virtual std::map<std::string, IResource*>& GetLoadedResources() = 0;

	template<class T>
	T* LoadResource(const std::string& path, ResourceLoadFlag load_flag, ResourceLoadOptions* options);
};

#include <typeinfo>
template<class T>
T* IResourceManager::LoadResource(const std::string& path, ResourceLoadFlag load_flag, ResourceLoadOptions* options)
{
	IResource* resource = NULL;

	TSResult res = LoadResourceInternal(path, load_flag, resource, options);

	return (T*)(void*)resource;
}