#pragma once

#include "resources/resource.hpp"

#include "formats/ts_model.hpp"

class TSModelResource : public IResource
{
public:
	virtual ~TSModelResource() = default;

	virtual TSModel& GetData() = 0;
};