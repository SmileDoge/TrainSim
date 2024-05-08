#pragma once

#include "modules/render/mesh.hpp"
#include "modules/render/material.hpp"

#define TS_MODEL_VERSION 1

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

enum TSKeyPositionType
{
	TS_KEY_POSITION_LINEAR_POS,
	TS_KEY_POSITION_SLERP_ROT,
	TS_KEY_POSITION_TCB_KEY,
};

struct TSKeyPosition
{
	TSKeyPositionType type;
	int frame;
	float x, y, z, w;
};

struct TSController
{
	std::vector<TSKeyPosition> key_positions;
};

struct TSAnimNode
{
	std::string name;
	std::vector<TSController> controllers;
};

struct TSAnimation
{
	int frame_count;
	int frame_rate;
	std::vector<TSAnimNode> anim_nodes;
};

struct TSModel
{
	std::vector<TSModel_Lod> lods;
	std::vector<glm::mat4> matrices;
	std::vector<std::string> matrices_name;
	std::vector<TSAnimation> animations;
};