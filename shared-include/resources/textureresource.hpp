#pragma once

#include "resources/resource.hpp"

#include "modules/render/texture.hpp"

class TextureResourceLoadOptions : public ResourceLoadOptions
{
public:
	bool ForceDXT;

	TextureResourceLoadOptions() : ForceDXT(false)
	{}
};

class TSTextureResource : public IResource
{
public:
	virtual ~TSTextureResource() = default;

	virtual ITexture* GetData() = 0;
};