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

TSResult CResourceManager::PrecacheResource(const std::string& path, ResourceLoadFlag loadflag)
{
	return TSResult();
}

TSResult CResourceManager::LoadResourceInternal(const std::string& path, ResourceLoadFlag loadflag, IResource*& resource)
{
	if (resources.find(path) != resources.end())
	{
		resource = resources[path];

		//g_Log->LogWarn("Return loaded %s", path.c_str());

		return TS_OK;
	}

	IFileStream* stream = g_FileSystem->OpenFile(path, "rb");
		
	if (stream == NULL)
		return TS_NOT_FOUND;

	IResourceFactory* factory = GetFactory(stream);

	if (factory == NULL)
	{
		delete stream;

		g_Log->LogError("ResourceFactory not found for [%s]", path);

		return TS_INVALID_OPERATION;
	}

	stream->Seek(6); // TS****

	TSResult res = factory->CreateResourceFromStream(stream, resource);

	if (res == TS_OK)
	{
		resource->SetFlag(RESOURCE_FLAG_DEFAULT);
		resource->SetType(factory->GetResourceType());
		resource->SetPath(path);

		resources[path] = resource;
	}

	delete stream;

	return res;
}

void CResourceManager::RegisterResourceFactory(IResourceFactory* factory)
{
	if (factory == NULL) return;

	ResourceHeader header = factory->GetHeader();
	ResourceType type = factory->GetResourceType();

	type_map[header] = type;
	factories[type] = factory;
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
