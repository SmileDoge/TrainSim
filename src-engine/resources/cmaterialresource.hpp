#pragma once

#include "modules/cresourcemanager.hpp"

#include "resources/materialresource.hpp"

class MaterialResourceFactory : public IResourceFactory
{
public:
	MaterialResourceFactory();
	~MaterialResourceFactory() override;

	virtual TSResult CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options);

	virtual ResourceType GetResourceType() { return RESOURCE_TYPE_MATERIAL; };
	virtual ResourceHeader GetHeader() { return GET_RESOURCE_HEADER("MATR"); };
};

class CTSMaterialResource : public TSMaterialResource
{
public:
	CTSMaterialResource();
	~CTSMaterialResource() override;

	virtual IMaterial* GetData();

	TSResult LoadFromStream(IFileStream* stream, MaterialResourceLoadOptions* options);
private:

	IMaterial* texture;
};