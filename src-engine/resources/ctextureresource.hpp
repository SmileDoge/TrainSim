#pragma once

#include "modules/cresourcemanager.hpp"

#include "resources/textureresource.hpp"

class TextureResourceFactory : public IResourceFactory
{
public:
	TextureResourceFactory();
	~TextureResourceFactory() override;

	virtual TSResult CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options);

	virtual ResourceType GetResourceType() { return RESOURCE_TYPE_TEXTURE; };
	virtual ResourceHeader GetHeader() { return GET_RESOURCE_HEADER("TEXT"); };
};

class CTSTextureResource : public TSTextureResource
{
public:
	CTSTextureResource();
	~CTSTextureResource() override;

	virtual ITexture* GetData();

	TSResult LoadFromStream(IFileStream* stream, TextureResourceLoadOptions* options);
private:

	ITexture* texture;
};