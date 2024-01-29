#pragma once

#include "glm/mat4x4.hpp"

class IMesh;
class IMaterial;

class RenderItem
{
public:
	RenderItem() : Mesh(NULL), Material(NULL), Transform() {}
	RenderItem(IMesh* mesh, IMaterial* material) : Mesh(mesh), Material(material), Transform() {}
	RenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4 transform) : Mesh(mesh), Material(material), Transform(transform) {}
	~RenderItem() {};

	IMesh* Mesh;
	IMaterial* Material;
	glm::mat4x4 Transform;
};