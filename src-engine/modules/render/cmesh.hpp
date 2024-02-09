#pragma once

#include "modules/render/mesh.hpp"

#include "glad/glad.h"

#include <vector>

class CMesh : public IMesh
{
public:
	CMesh();
	~CMesh() override;

	virtual void Render();
	virtual void SetData(float* vertices, int num_vertices, USHORT* indices, int num_indices);

	virtual std::string& GetName();
	virtual void SetName(const std::string& name);

	int ref_count;
private:
	std::string name;

	unsigned int VAO, VBO, EBO;

	USHORT indices_num;
	int vertices_num;
};

class CMeshManager : public IMeshManager
{
public:
	CMeshManager();
	~CMeshManager() override;

	virtual IMesh* CreateMesh();

	virtual void DeleteMesh(IMesh* mesh);

	virtual int MeshReferenceIncrement(IMesh* mesh);
	virtual int MeshReferenceDecrement(IMesh* mesh);

private:
	std::vector<IMesh*> meshes;
};

extern CMeshManager* g_MeshManager;