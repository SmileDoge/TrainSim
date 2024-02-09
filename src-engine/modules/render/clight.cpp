#include "clight.hpp"

CLight::CLight() : position(0.0f), ambient(0.1f), diffuse(1.0f), specular(1.0f)
{

}

CLight::~CLight()
{

}

void CLight::SetType(LightType type)
{
	this->type = type;
}

LightType CLight::GetType()
{
	return type;
}

void CLight::SetPosition(glm::vec3& pos)
{
	position = pos;
}

glm::vec3& CLight::GetPosition()
{
	return position;
}

void CLight::SetAmbient(glm::vec3& ambient)
{
	this->ambient = ambient;
}

glm::vec3& CLight::GetAmbient()
{
	return ambient;
}

void CLight::SetDiffuse(glm::vec3& diffuse)
{
	this->diffuse = diffuse;
}

glm::vec3& CLight::GetDiffuse()
{
	return diffuse;
}

void CLight::SetSpecular(glm::vec3& specular)
{
	this->specular = specular;
}

glm::vec3& CLight::GetSpecular()
{
	return specular;
}
