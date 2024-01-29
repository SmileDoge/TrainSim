#include "cshader.hpp"
#include "glad/glad.h"

#include "modules/crendermodule.hpp"
#include "modules/clogmodule.hpp"

CShader::CShader() : shader_program(0), vertex_code(NULL), fragment_code(NULL), name("CShader")
{

}

CShader::~CShader()
{
	glDeleteShader(shader_program);
}

void CShader::SetVertexShader(const char* code)
{
	vertex_code = code;
}

void CShader::SetFragmentShader(const char* code)
{
	fragment_code = code;
}

int CShader::Compile(char* result)
{
	int res;

	int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);
	if (!res)
		glGetShaderInfoLog(vertex, 512, NULL, result);


	int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);
	if (!res)
		glGetShaderInfoLog(fragment, 512, NULL, result);


	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex);
	glAttachShader(shader_program, fragment);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
	if (!res)
		glGetProgramInfoLog(shader_program, 512, NULL, result);


	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (res && g_Render->ExtensionCanLabel())
		glObjectLabel(GL_PROGRAM, shader_program, -1, name.c_str());

	return res;
}

void CShader::Use()
{
	glUseProgram(shader_program);
}

void CShader::SetInt(char* name, int value)
{
	int loc = GetUniformLocation(name);

	Use();

	glUniform1i(loc, value);
}

void CShader::SetFloat(char* name, float value)
{
	int loc = GetUniformLocation(name);

	Use();

	glUniform1f(loc, value);
}

void CShader::SetVec3(char* name, glm::vec3& value)
{
	int loc = GetUniformLocation(name);

	Use();

	glUniform3f(loc, value.x, value.y, value.z);
}

void CShader::SetMat4x4(char* name, glm::mat4x4& value)
{
	int loc = GetUniformLocation(name);

	Use();

	glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

int CShader::GetUniformLocation(char* name)
{
	return glGetUniformLocation(shader_program, name);
}

void CShader::SetName(const std::string& name)
{
	this->name = name;
}

std::string& CShader::GetName()
{
	return name;
}

/*

	virtual void SetVertexShader(const char* vertex_code);
	virtual void SetFragmentShader(const char* fragment_code);
	virtual TSResult Compile(char* result = NULL);

	virtual void Use();

	virtual std::string& GetName();
	virtual void SetName(std::string name);

	virtual void SetInt(char* name, int value);
	virtual void SetFloat(char* name, float value);
	virtual void SetVec3(char* name, glm::vec3 value);
	virtual void SetMat4x4(char* name, glm::mat4x4 value);

	virtual int GetUniformLocation(char* name);
	*/