#pragma once

/*
#include "global.hpp"

#include <string>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct TSModel_Vertex
{
	glm::highp_vec3 position;
	glm::highp_vec3 normal;
	glm::highp_vec2 texcoord;
};

struct TSModel_Primitive
{
	int iHierarchy;
	std::vector<USHORT> triangles;
	std::string material;
	int options;
};

struct TSModel_SubObject
{
	std::vector<TSModel_Primitive> primitives;
	std::vector<TSModel_Vertex> vertices;
};

struct TSModel_Lod
{
	float distance;
	std::vector<TSModel_SubObject> subobjects;
	std::vector<int> hierarchy;
};

struct TSModel
{
	std::vector<TSModel_Lod> lods;
	std::vector<glm::mat4> matrices;
	std::vector<std::string> matrices_name;
};
*/

#include "modules/render/mesh.hpp"
#include "modules/render/material.hpp"

struct TSModel_Primitive
{
	unsigned int sort_index;
	int iHierarchy;
	IMesh* mesh;
	IMaterial* material;
};

struct TSModel_Lod
{
	float distance;
	float sphere_radius;
	std::vector<TSModel_Primitive> primitives;
	std::vector<int> hierarchy;
};

struct TSModel
{
	std::vector<TSModel_Lod> lods;
	std::vector<glm::mat4> matrices;
	std::vector<std::string> matrices_name;
};