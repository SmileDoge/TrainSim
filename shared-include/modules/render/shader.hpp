#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include <string>

#include "global.hpp"

class IShader
{
public:
	virtual ~IShader() = default;

	virtual void SetVertexShader(const char* vertex_code) = 0;
	virtual void SetFragmentShader(const char* fragment_code) = 0;
	virtual int Compile(char* result = NULL) = 0;

	virtual void Use() = 0;

	virtual std::string& GetName() = 0;
	virtual void SetName(const std::string& name) = 0;

	virtual void SetInt(char* name, int value) = 0;
	virtual void SetFloat(char* name, float value) = 0;
	virtual void SetVec3(char* name, glm::vec3& value) = 0;
	virtual void SetVec4(char* name, glm::vec4& value) = 0;
	virtual void SetMat4x4(char* name, glm::mat4x4& value) = 0;

	virtual void SetInt(int loc, int value) = 0;
	virtual void SetFloat(int loc, float value) = 0;
	virtual void SetVec3(int loc, glm::vec3& value) = 0;
	virtual void SetVec4(int loc, glm::vec4& value) = 0;
	virtual void SetMat4x4(int loc, glm::mat4x4& value) = 0;

	virtual void SetModelMat(glm::mat4& value) = 0;
	virtual void SetViewMat(glm::mat4& value) = 0;
	virtual void SetProjMat(glm::mat4& value) = 0;

	virtual int GetUniformLocation(char* name) = 0;
};