#pragma once

#include "modules/render/shader.hpp"

class CShader : public IShader
{
public:
	CShader();
	~CShader() override;

	virtual void SetVertexShader(const char* vertex_code);
	virtual void SetFragmentShader(const char* fragment_code);
	virtual int Compile(char* result = NULL);

	virtual void Use();

	virtual std::string& GetName();
	virtual void SetName(const std::string& name);

	virtual void SetInt(char* name, int value);
	virtual void SetFloat(char* name, float value);
	virtual void SetVec3(char* name, glm::vec3& value);
	virtual void SetMat4x4(char* name, glm::mat4x4& value);

	virtual int GetUniformLocation(char* name);

private:
	int shader_program;
	
	std::string name;

	const char* vertex_code;
	const char* fragment_code;
};