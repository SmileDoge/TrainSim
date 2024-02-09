#pragma once

#include "global.hpp"

#include "module.hpp"

#include "resources/resource.hpp"
#include "resources/modelresource.hpp"

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

	virtual TSResult CreateResourceFromStream(IFileStream* stream, IResource*& resource) = 0;

	virtual ResourceType GetResourceType() = 0;
	virtual ResourceHeader GetHeader() = 0;
};

class IResourceManager : public IModule
{
public:
	virtual ~IResourceManager() = default;

	virtual TSResult PrecacheResource(const std::string& path, ResourceLoadFlag loadflag) = 0;
	virtual TSResult LoadResourceInternal(const std::string& path, ResourceLoadFlag loadflag, IResource*& resource) = 0;

	virtual void RegisterResourceFactory(IResourceFactory* factory) = 0;

	template<class T>
	T* LoadResource(const std::string& path, ResourceLoadFlag loadflag);
};

#include <typeinfo>
template<class T>
T* IResourceManager::LoadResource(const std::string& path, ResourceLoadFlag loadflag)
{
	IResource* resource = NULL;

	LoadResourceInternal(path, loadflag, resource);

	return (T*)(void*)resource;
}