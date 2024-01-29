#pragma once

#include "global.hpp"
#include <string>

class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual void Render() = 0;
	virtual void SetData(float* vertices, int num_vertices, USHORT* indices, int num_indices) = 0;

	virtual std::string& GetName() = 0;
	virtual void SetName(const std::string& name) = 0;
};

class IMeshManager
{
public:
	virtual ~IMeshManager() = default;

	virtual IMesh* CreateMesh() = 0;

	virtual void DeleteMesh(IMesh* mesh) = 0;

	virtual int MeshReferenceIncrement(IMesh* mesh) = 0;
	virtual int MeshReferenceDecrement(IMesh* mesh) = 0;
};