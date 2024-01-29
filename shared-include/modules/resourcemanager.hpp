#pragma once
#include "module.hpp"
#include "resources/resource.hpp"

#include <vector>
#include <map>
#include <string>

enum ResourceType
{
	RESOURCE_TYPE_UNKNOWN = 0,

	RESOURCE_TYPE_TEXT = 1,
	RESOURCE_TYPE_JSON,

	RESOURCE_TYPE_MODEL,
	RESOURCE_TYPE_TEXTURE,
	RESOURCE_TYPE_MATERIAL,
	RESOURCE_TYPE_SHADER,
};

class IResourceFactory : public IModule
{
public:
	virtual ~IResourceFactory() = default;

	virtual void AddResource(std::string name, IResource* resource) = 0;
	
	virtual void 

private:
	std::vector<IResource*> resources;
	std::map<ResourceType, std::map<std::string, IResource*>> resources_named;
};
