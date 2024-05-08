#pragma once

#include "resources/resource.hpp"

#include "modules/render/material.hpp"

class MaterialResourceLoadOptions : public ResourceLoadOptions
{
public:
	MaterialResourceLoadOptions()
	{}
};

class TSMaterialResource : public IResource
{
public:
	virtual ~TSMaterialResource() = default;

	virtual IMaterial* GetData() = 0;
};