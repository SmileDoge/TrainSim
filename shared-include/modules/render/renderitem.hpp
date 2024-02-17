#pragma once

#include "glm/mat4x4.hpp"

class IMesh;
class IMaterial;

class RenderItem
{
public:
	RenderItem() : Mesh(NULL), Material(NULL), Transform(), SortIndex(0) {}
	RenderItem(IMesh* mesh, IMaterial* material) : Mesh(mesh), Material(material), Transform(), SortIndex(0) {}
	RenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4 transform) : Mesh(mesh), Material(material), Transform(transform), SortIndex(0) {}
	RenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4 transform, unsigned int sort_index) : Mesh(mesh), Material(material), Transform(transform), SortIndex(sort_index) {}
	~RenderItem() {};

	IMesh* Mesh;
	IMaterial* Material;
	glm::mat4x4 Transform;
	unsigned int SortIndex;
};