#pragma once

#include "modules/cresourcemanager.hpp"

#include "resources/modelresource.hpp"

#include "resources/textureresource.hpp"

class ModelResourceFactory : public IResourceFactory
{
public:
	ModelResourceFactory();
	~ModelResourceFactory() override;

	virtual TSResult CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options);

	virtual ResourceType GetResourceType() { return RESOURCE_TYPE_MODEL; };
	virtual ResourceHeader GetHeader() { return GET_RESOURCE_HEADER("MODL"); };
};

class CTSModelResource : public TSModelResource
{
public:
	CTSModelResource();
	~CTSModelResource() override;

	virtual TSModel& GetData();

	TSResult LoadFromStream(IFileStream* stream, ModelResourceLoadOptions* options);
private:

	std::vector<TSTextureResource*> textures;

	TSModel model;
};