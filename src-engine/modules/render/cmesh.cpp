#include "cmesh.hpp"

#include "glad/glad.h"

#include "modules/crendermodule.hpp"

CMesh::CMesh() : indices_num(0), vertices_num(0), ref_count(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

CMesh::~CMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void CMesh::Render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	if (indices_num > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, indices_num, GL_UNSIGNED_SHORT, 0);

	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices_num);
	}
}

// XYZ Vertex Pos - 3 // 0
// XYZ Vertex Normal - 3 // 3
// ST Vertex TexUV - 2 // 6

void CMesh::SetData(float* vertices, int num_vertices, USHORT* indices, int num_indices)
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_vertices * 8), vertices, GL_STATIC_DRAW);

	if (num_indices > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(USHORT) * num_indices, indices, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	indices_num = num_indices;
	vertices_num = num_vertices;
}

void CMesh::SetName(const std::string& name)
{
	this->name = name;

	if (g_Render->ExtensionCanLabel())
	{
		glObjectLabel(GL_VERTEX_ARRAY, VAO, -1, (name + " VAO").c_str());
		glObjectLabel(GL_BUFFER, VBO, -1, (name + " VBO").c_str());
		glObjectLabel(GL_BUFFER, EBO, -1, (name + " EBO").c_str());
	}
}

std::string& CMesh::GetName()
{
	return name;
}

CMeshManager* g_MeshManager = NULL;

CMeshManager::CMeshManager() : meshes()
{
	g_MeshManager = this;
}

CMeshManager::~CMeshManager()
{
	for (auto mesh : meshes)
		delete mesh;
}

IMesh* CMeshManager::CreateMesh()
{
	CMesh* mesh = new CMesh();

	meshes.push_back(mesh);

	return mesh;
}

void CMeshManager::DeleteMesh(IMesh* mesh)
{
	if (std::find(meshes.begin(), meshes.end(), mesh) == meshes.end())
		return;

	delete mesh;
	
	meshes.erase(std::find(meshes.begin(), meshes.end(), mesh));
}

int CMeshManager::MeshReferenceIncrement(IMesh* mesh)
{
	return ++((CMesh*)mesh)->ref_count;
}

int CMeshManager::MeshReferenceDecrement(IMesh* mesh)
{
	int new_count = --((CMesh*)mesh)->ref_count;

	if (new_count <= 0)
		DeleteMesh(mesh);

	return new_count;
}
/*

	virtual IMesh* CreateMesh();

	virtual void DeleteMesh(IMesh* mesh);

	virtual int MeshReferenceIncrement(IMesh* mesh);
	virtual int MeshReferenceDecrement(IMesh* mesh);
*/