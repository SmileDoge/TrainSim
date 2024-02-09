#pragma once
#include "global.hpp"

#include <string>

enum ResourceFlag
{
	RESOURCE_FLAG_DEFAULT  = 0,
	RESOURCE_FLAG_STATIC   = 1 << 0,
	RESOURCE_FLAG_LOADING  = 1 << 1,
};

enum ResourceLoadFlag
{
	RESOURCE_LOAD_FLAG_DEFAULT = 0,
	RESOURCE_LOAD_FLAG_SYNC    = 1 << 0,
	RESOURCE_LOAD_FLAG_STATIC  = 1 << 1,
};

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

class IResource
{
public:
	virtual ~IResource() = default;

	ResourceType GetType() { return type; };
	void SetType(ResourceType type) { this->type = type; };

	ResourceFlag GetFlag() { return flag; };
	void SetFlag(ResourceFlag flag) { this->flag = flag; };

	std::string& GetPath() { return path; };
	void SetPath(const std::string& path) { this->path = path; };

private:
	ResourceType type;
	ResourceFlag flag;

	std::string path;
};

/*
template <class T>
class ResourceRef
{
public:
	ResourceRef()
	{
		data = 0;
	}

	ResourceRef(T* resource)
	{
		data = resource;
	}

	~ResourceRef()
	{
		if (data == 0) return;
	}

private:
	T* data;
};
*/