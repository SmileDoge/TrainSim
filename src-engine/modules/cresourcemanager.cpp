#include "cresourcemanager.hpp"

#include "modules/cfilesystem.hpp"
#include "modules/clogmodule.hpp"

CResourceManager* g_ResourceManager = NULL;

CResourceManager::CResourceManager()
{
	g_ResourceManager = this;
}

CResourceManager::~CResourceManager()
{
}

TSResult CResourceManager::PrecacheResource(const std::string& path, ResourceLoadFlag load_flag)
{
	return TSResult();
}

TSResult CResourceManager::LoadResourceInternal(const std::string& path, ResourceLoadFlag load_flag, IResource*& resource, ResourceLoadOptions* options)
{
	if (resources.find(path) != resources.end())
	{
		resource = resources[path];

		return TS_OK;
	}

	IFileStream* stream = g_FileSystem->OpenFile(path, "rb");
		
	if (stream == NULL)
	{
		g_Log->LogError("LoadResourceInternal | Error! [File not found]\n\tPath: %s", path.c_str());

		return TS_NOT_FOUND;
	}

	IResourceFactory* factory = GetFactory(stream);

	if (factory == NULL)
	{
		delete stream;

		g_Log->LogError("LoadResourceInternal | Error! [Resource factory not found]\n\tPath: %s", path.c_str());

		return TS_INVALID_OPERATION;
	}

	stream->Seek(6); // TS****

	TSResult res = TS_ERROR;

	try
	{
		res = factory->CreateResourceFromStream(stream, resource, options);
	}
	catch (std::exception& err)
	{
		g_Log->LogError("LoadResourceInternal | Error! [CreateResource Exception]\n\tFile: %s\n\tError: %s", stream->GetPath().c_str(), err.what());

#ifdef _DEBUG
		throw err;
#endif
	}


	if (res == TS_OK)
	{
		ResourceFlag flag = RESOURCE_FLAG_DEFAULT;

		if ((load_flag & RESOURCE_LOAD_FLAG_STATIC) != 0)
			flag = flag | RESOURCE_FLAG_STATIC;

		resource->SetFlag(flag);
		resource->SetType(factory->GetResourceType());
		resource->SetPath(path);
		resource->SetRefCount(0);

		resources[path] = resource;
	}
	else
	{
		g_Log->LogError("CreateResourceFromStream return %d", res);
	}

	delete stream;

	return res;
}

void CResourceManager::IncrementRefResource(IResource* resource)
{
	if ((resource->GetFlag() & RESOURCE_FLAG_STATIC) != 0)
		return;

	resource->SetRefCount(resource->GetRefCount() + 1);
}

void CResourceManager::DecrementRefResource(IResource* resource)
{
	if ((resource->GetFlag() & RESOURCE_FLAG_STATIC) != 0)
		return;

	int ref_count = resource->GetRefCount();

	ref_count--;

	if (ref_count <= 0)
	{
		resources.erase(resource->GetPath());

		delete resource;

		return;
	}

	resource->SetRefCount(ref_count);
}

TSResult CResourceManager::UnloadResource(IResource* resource)
{
	if ((resource->GetFlag() & RESOURCE_FLAG_STATIC) != 0)
		return TS_ALREADY_USED;

	resources.erase(resource->GetPath());

	delete resource;
}

void CResourceManager::RegisterResourceFactory(IResourceFactory* factory)
{
	if (factory == NULL) return;

	ResourceHeader header = factory->GetHeader();
	ResourceType type = factory->GetResourceType();

	type_map[header] = type;
	factories[type] = factory;
}

std::map<std::string, IResource*>& CResourceManager::GetLoadedResources()
{
	return resources;
}

IResourceFactory* CResourceManager::GetFactory(IFileStream* stream)
{
	char header[6]; // TS*, TSMODL
	ResourceHeader res_header;

	stream->ReadBytes(header, 6);

	if (header[0] != 'T' || header[1] != 'S')
		return NULL;

	res_header = *(int*)(void*)(&header[2]);

	if (type_map.find(res_header) == type_map.end())
		return NULL;

	ResourceType type = type_map[res_header];

	IResourceFactory* factory = factories[type];

	return factory;
}
