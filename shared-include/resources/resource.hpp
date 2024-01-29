#pragma once
#include "global.hpp"

enum ResourceFlag
{
	RESOURCE_FLAG_DEFAULT = 0x00,
	RESOURCE_FLAG_STATIC  = 0x01,
};

class IResource
{
public:
	virtual ~IResource() = 0;

	virtual USHORT GetID() { return id; };
	ResourceFlag GetFlag() { return flag; };
	void SetFlag(ResourceFlag flag) { this->flag = flag; };



private:
	USHORT id;
	ResourceFlag flag;
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