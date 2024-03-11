#pragma once

#include "modules/render/sky.hpp"

#include "modules/render/materials/skymaterial.hpp"

#include "modules/render/mesh.hpp"

#include <string>
#include <vector>

struct VertexPositionNormalTexture
{
	glm::highp_vec3 Position;
	glm::highp_vec3 Normal;
	glm::highp_vec2 TexCoord;
};

constexpr int SKY_RADIUS = 6000;
constexpr int SKY_SIDES = 24;
constexpr short SKY_LEVELS = 6;

class SkyMesh : public IMesh
{
public:
	SkyMesh();
	~SkyMesh() override;

	virtual void Render();
	virtual void SetData(float* vertices, int num_vertices, USHORT* indices, int num_indices) {};

	virtual std::string& GetName();
	virtual void SetName(const std::string& name);

	int DrawIndex;
private:
	void DomeVertexList(int index, int radius, float oblate);
	void DomeTriangleList(short index, short pass);
	void MoonLists(int vertex_index, int iIndex);

	void CreateBuffers();

	std::string name;
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<USHORT> indices;

	int sky_radius, sky_sides, cloud_dome_radius_diff;

	short sky_levels;

	int num_vertices, num_indices;

	unsigned int VAO, VBO, EBO;
};

class CSky : public ISky
{
public:
	CSky();
	~CSky() override;

	virtual void Render();

	virtual IMesh* GetSkyMesh();
	virtual IMaterial* GetSkyMaterial();
private:

	RenderItem sky_item;
	SkyMesh* sky_mesh;
	SkyMaterial* sky_material;
};

extern CSky* g_Sky;