#include "csky.hpp"

#include "modules/clogmodule.hpp"
#include "modules/crendermodule.hpp"
#include "modules/cfilesystem.hpp"

#include "glad/glad.h"

#include "stb_image.h"

CSky* g_Sky = NULL;


void LoadTexture(ITexture* texture, const std::string& path)
{
	int width, height, channels;

	size_t file_len;
	uint8_t* file_data = g_FileSystem->ReadFile(path, &file_len);

	uint8_t* data = stbi_load_from_memory(file_data, file_len, &width, &height, &channels, 0);

	delete file_data;

	texture->SetFilter(TEXTURE_FILTER_LINEAR, TEXTURE_FILTER_LINEAR);
	texture->SetWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
	texture->SetData(width, height, channels == 4 ? TEXTURE_FORMAT_RGBA : TEXTURE_FORMAT_RGB, data);
	//texture->GenerateMipmap();

	stbi_image_free(data);

	return;
}

CSky::CSky()
{
	g_Sky = this;

	sky_mesh = new SkyMesh();
	sky_material = (SkyMaterial*)g_Render->GetMaterialManager()->CreateMaterial("sky_material", "sky_shader");

	sky_mesh->SetName("Sky");

	sky_item = RenderItem(sky_mesh, sky_material, glm::mat4(1.0f));

	ITexture* sky_dome = g_Render->GetTextureManager()->CreateTexture("SkyDome");
	ITexture* stars = g_Render->GetTextureManager()->CreateTexture("StarsMap");

	LoadTexture(sky_dome, "data/images/SkyDome1.png");
	LoadTexture(stars, "data/images/Starmap_N.png");

	sky_material->SetTexture(MATERIAL_SKY_TEXTURE_SKY_DOME, sky_dome);
	sky_material->SetTexture(MATERIAL_SKY_TEXTURE_STAR_MAP_N, stars);
}

CSky::~CSky()
{
	delete sky_mesh;

	g_Render->GetMaterialManager()->DeleteMaterial(sky_material);
}

void CSky::Render()
{
	auto camera = g_Render->GetCamera();

	glm::mat4& mat_view = camera->GetViewMatrix();
	glm::mat4& mat_proj = camera->GetSkyProjectionMatrix();

	auto loc = camera->GetLocation();

	auto pos = glm::vec3(loc.x, loc.y - 100, loc.z);

	sky_item.Transform = glm::translate(glm::mat4(1.0f), pos);

	//sky_material->PreRender(NULL);
	sky_material->Render(sky_item, mat_view, mat_proj);
	sky_material->PostRender();
}

IMesh* CSky::GetSkyMesh()
{
	return sky_mesh;
}

IMaterial* CSky::GetSkyMaterial()
{
	return sky_material;
}

SkyMesh::SkyMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	sky_levels = SKY_LEVELS;
	sky_radius = SKY_RADIUS;
	sky_sides = SKY_SIDES;

	cloud_dome_radius_diff = 600;

	num_vertices = 4 + 2 * ((sky_levels + 1) * sky_sides + 1);
	num_indices = 6 + 2 * (sky_sides * 6 * sky_levels + 3 * sky_sides);

	vertices = std::vector<VertexPositionNormalTexture>(num_vertices);
	indices = std::vector<USHORT>(num_indices);

	DomeVertexList(0, sky_radius, 1.0f);
	DomeTriangleList(0, 0);

	DomeVertexList((num_vertices - 4) / 2, sky_radius - cloud_dome_radius_diff, 0.4f);
	DomeTriangleList((short)((num_indices - 6) / 2), 1);

	MoonLists(num_vertices - 5, num_indices - 6);
	CreateBuffers();
}

SkyMesh::~SkyMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void SkyMesh::Render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	if (DrawIndex == 1)
		glDrawElements(GL_TRIANGLES, (num_indices - 6) / 6 * 3, GL_UNSIGNED_SHORT, 0);
	else if (DrawIndex == 2)
		glDrawElements(GL_TRIANGLES, num_indices - 6, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(GLuint)));
	else if (DrawIndex == 3)
		glDrawElements(GL_TRIANGLES, (num_indices - 6) / 2 * 3, GL_UNSIGNED_SHORT, (void*)((6 / 2) * sizeof(GLuint)));
}

#define RADIANS(x) glm::radians((float)x)

void SkyMesh::DomeVertexList(int index, int radius, float oblate)
{
	int vertexIndex = index;

	int texDivisor = (oblate == 1.0f) ? sky_levels : sky_levels + 1;

	for (int i = 0; i <= sky_levels; i++)
	{
		float y = sinf(RADIANS((360 / sky_sides) * (i - 1))) * radius * oblate;
		float yRadius = radius * cosf(RADIANS((360 / sky_sides) * (i - 1)));

		for (int j = 0; j < sky_sides; j++)
		{
			float x = cosf(RADIANS((360 / sky_sides) * (sky_sides - j))) * yRadius;
			float z = sinf(RADIANS((360 / sky_sides) * (sky_sides - j))) * yRadius;

			float uvRadius;

			uvRadius = 0.5f - (float)(0.5f * (i - 1)) / texDivisor;
			float uv_u = 0.5f - (cosf(RADIANS((360 / sky_sides) * (sky_sides - j))) * uvRadius);
			float uv_v = 0.5f - (sinf(RADIANS((360 / sky_sides) * (sky_sides - j))) * uvRadius);

			vertices[vertexIndex].Position = glm::vec3(x, y, z);
			vertices[vertexIndex].TexCoord = glm::vec2(uv_u, uv_v);
			vertices[vertexIndex].Normal = glm::normalize(glm::vec3(x, y, z));
			vertexIndex++;
		}
	}

	vertices[vertexIndex].Position = glm::vec3(0, radius, 0);
	vertices[vertexIndex].Normal = glm::vec3(0, 1, 0);
	vertices[vertexIndex].TexCoord = glm::vec2(0.5f, 0.5f);
}

void SkyMesh::DomeTriangleList(short index, short pass)
{
	short iIndex = index;
	short baseVert = pass * ((num_vertices - 4) / 2);

	for (int i = 0; i < sky_levels; i++)
		for (int j = 0; j < sky_sides; j++)
		{
			short sw = baseVert + (j + i * sky_sides);
			short nw = sw + sky_sides;
			short ne = nw + 1;

			short se = sw + 1;

			if (((i & 1) == (j & 1)))
			{
				indices[iIndex++] = sw;
				indices[iIndex++] = ((ne - baseVert) % sky_sides == 0) ? (ne - sky_sides) : ne;
				indices[iIndex++] = nw;
				indices[iIndex++] = sw;
				indices[iIndex++] = ((se - baseVert) % sky_sides == 0) ? (se - sky_sides) : se;
				indices[iIndex++] = ((ne - baseVert) % sky_sides == 0) ? (ne - sky_sides) : ne;
			}
			else
			{
				indices[iIndex++] = sw;
				indices[iIndex++] = ((se - baseVert) % sky_sides == 0) ? (se - sky_sides) : se;
				indices[iIndex++] = nw;
				indices[iIndex++] = ((se - baseVert) % sky_sides == 0) ? (se - sky_sides) : se;
				indices[iIndex++] = ((ne - baseVert) % sky_sides == 0) ? (ne - sky_sides) : ne;
				indices[iIndex++] = nw;
			}
		}

	for (int i = 0; i < sky_sides; i++)
	{
		short sw = baseVert + (sky_sides * sky_levels) + i;
		short se = sw + 1;

		indices[iIndex++] = sw;
		indices[iIndex++] = ((se - baseVert) % sky_sides == 0) ? (se - sky_sides) : se;
		indices[iIndex++] = (baseVert + (num_vertices - 5) / 2);
	}
}

void SkyMesh::MoonLists(int vertex_index, int iIndex)
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			vertex_index++;
			vertices[vertex_index].Position = glm::vec3(i, j, 0);
			vertices[vertex_index].Normal = glm::vec3(0, 0, 1);
			vertices[vertex_index].TexCoord = glm::vec2(i, j);
		}

	short msw = num_vertices - 4;
	short mnw = msw + 1;
	short mse = mnw + 1;
	short mne = mse + 1;

	indices[iIndex++] = msw;
	indices[iIndex++] = mnw;
	indices[iIndex++] = mse;
	indices[iIndex++] = mse;
	indices[iIndex++] = mnw;
	indices[iIndex++] = mne;
}

void SkyMesh::CreateBuffers()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_vertices * 8), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(USHORT) * num_indices, indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void SkyMesh::SetName(const std::string& name)
{
	this->name = name;

	if (g_Render->ExtensionCanLabel())
	{
		glObjectLabel(GL_VERTEX_ARRAY, VAO, -1, (name + " VAO").c_str());
		glObjectLabel(GL_BUFFER, VBO, -1, (name + " VBO").c_str());
		glObjectLabel(GL_BUFFER, EBO, -1, (name + " EBO").c_str());
	}
}

std::string& SkyMesh::GetName()
{
	return name;
}