#pragma once

#include "resources/resource.hpp"

#include "modules/render/texture.hpp"

class TSTextureResource : public IResource
{
public:
	virtual ~TSTextureResource() = default;

	virtual ITexture* GetData() = 0;
};