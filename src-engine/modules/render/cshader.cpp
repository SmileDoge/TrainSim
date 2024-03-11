#include "cshader.hpp"
#include "glad/glad.h"

#include "modules/crendermodule.hpp"
#include "modules/clogmodule.hpp"

CShader::CShader() : shader_program(0), vertex_code(NULL), fragment_code(NULL), name("CShader")
{
	model_uniform = -1;
	view_uniform = -1;
	proj_uniform = -1;
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

	if (vertex_code == NULL)
	{
		g_Log->LogError("CShader::Compile - Vertex Code is NULL");
		return 0;
	}

	if (fragment_code == NULL)
	{
		g_Log->LogError("CShader::Compile - Fragment Code is NULL");
		return 0;
	}

	int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);
	if (!res && result)
		glGetShaderInfoLog(vertex, 512, NULL, result);
	else if (!res && !result)
	{
		char infoLog[1024];
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		g_Log->LogError("CShader::Compile - GL_VERTEX_SHADER\n%s\n-----", infoLog);
	}


	int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);

	if (!res && result)
	{
		glGetShaderInfoLog(fragment, 512, NULL, result);
	}
	else if (!res && !result)
	{
		char infoLog[1024];
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		g_Log->LogError("CShader::Compile - GL_FRAGMENT_SHADER\n%s\n-----", infoLog);
	}


	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex);
	glAttachShader(shader_program, fragment);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &res);
	if (!res && result)
		glGetProgramInfoLog(shader_program, 512, NULL, result);
	else if (!res && !result)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		g_Log->LogError("CShader::Compile - GL_PROGRAM\n%s\n-----", infoLog);
	}

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

	SetInt(loc, value);
}

void CShader::SetInt(int loc, int value)
{
	glUniform1i(loc, value);
}

void CShader::SetFloat(char* name, float value)
{
	int loc = GetUniformLocation(name);

	SetFloat(loc, value);
}

void CShader::SetFloat(int loc, float value)
{
	glUniform1f(loc, value);
}

void CShader::SetVec3(char* name, glm::vec3& value)
{
	int loc = GetUniformLocation(name);

	SetVec3(loc, value);
}

void CShader::SetVec3(int loc, glm::vec3& value)
{
	glUniform3f(loc, value.x, value.y, value.z);
}

void CShader::SetVec4(char* name, glm::vec4& value)
{
	int loc = GetUniformLocation(name);

	SetVec4(loc, value);
}

void CShader::SetVec4(int loc, glm::vec4& value)
{
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void CShader::SetMat4x4(char* name, glm::mat4x4& value)
{
	int loc = GetUniformLocation(name);

	SetMat4x4(loc, value);
}

void CShader::SetMat4x4(int loc, glm::mat4x4& value)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void CShader::SetModelMat(glm::mat4& value)
{
	if (model_uniform == -1) model_uniform = GetUniformLocation("model_mat");

	SetMat4x4(model_uniform, value);
}

void CShader::SetViewMat(glm::mat4& value)
{
	if (view_uniform == -1) view_uniform = GetUniformLocation("view_mat");

	SetMat4x4(view_uniform, value);
}

void CShader::SetProjMat(glm::mat4& value)
{
	if (proj_uniform == -1) proj_uniform = GetUniformLocation("proj_mat");

	SetMat4x4(proj_uniform, value);
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