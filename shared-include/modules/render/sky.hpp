#pragma once

#include "modules/render/mesh.hpp"
#include "modules/render/material.hpp"

class ISky
{
public:
	virtual ~ISky() = default;

	virtual void Render() = 0;

	virtual IMesh* GetSkyMesh() = 0;
	virtual IMaterial* GetSkyMaterial() = 0;
};