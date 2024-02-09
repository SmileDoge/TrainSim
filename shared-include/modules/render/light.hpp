#pragma once

#include "glm/vec3.hpp"

enum LightType
{
	LIGHT_TYPE_POINT,
	LIGHT_TYPE_SPOT,
	LIGHT_TYPE_DIRECTIONAL,
};

class ILight
{
public:
	virtual ~ILight() = default;

	virtual void SetType(LightType type) = 0;
	virtual LightType GetType() = 0;

	virtual void SetPosition(glm::vec3& pos) = 0;
	virtual glm::vec3& GetPosition() = 0;

	virtual void SetAmbient(glm::vec3& ambient) = 0;
	virtual glm::vec3& GetAmbient() = 0;

	virtual void SetDiffuse(glm::vec3& diffuse) = 0;
	virtual glm::vec3& GetDiffuse() = 0;
	
	virtual void SetSpecular(glm::vec3& specular) = 0;
	virtual glm::vec3& GetSpecular() = 0;
};