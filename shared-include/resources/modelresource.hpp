#pragma once

#include "modules/filesystem.hpp"
#include "resources/resource.hpp"
#include "formats/ts_model.hpp"

#include <string>

/*
enum ModelResourceLoadTextureFrom
{
	MODEL_LOAD_TEXTURE_FROM_GLOBAL,
	MODEL_LOAD_TEXTURE_FROM_ROUTE,
	MODEL_LOAD_TEXTURE_FROM_TRAIN,

	MODEL_LOAD_TEXTURE_FROM_CURRENT_DIRECTORY = MODEL_LOAD_TEXTURE_FROM_TRAIN,
};
*/

class ModelResourceLoadOptions : public ResourceLoadOptions
{
public:
	FindFileFrom TextureLoadFrom;
	std::string RootPath;

	ModelResourceLoadOptions() : TextureLoadFrom(FIND_FILE_FROM_CURRENT_DIRECTORY), RootPath("")
	{}
};

class TSModelResource : public IResource
{
public:
	virtual ~TSModelResource() = default;

	virtual TSModel& GetData() = 0;
};