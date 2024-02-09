#pragma once

#include "modules/render/light.hpp"

class CLight : public ILight
{
public:
	CLight();
	~CLight() override;

	virtual void SetType(LightType type);
	virtual LightType GetType();

	virtual void SetPosition(glm::vec3& pos);
	virtual glm::vec3& GetPosition();

	virtual void SetAmbient(glm::vec3& ambient);
	virtual glm::vec3& GetAmbient();

	virtual void SetDiffuse(glm::vec3& diffuse);
	virtual glm::vec3& GetDiffuse();

	virtual void SetSpecular(glm::vec3& specular);
	virtual glm::vec3& GetSpecular();

private:
	glm::vec3 position, ambient, diffuse, specular;

	LightType type;
};