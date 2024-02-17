#pragma once

#include <string>
#include <vector>
#include "shader.hpp"
#include "texture.hpp"
#include "renderitem.hpp"

enum MaterialStandardParam
{
	MATERIAL_ALBEDO_TEXTURE,

	MATERIAL_MSTS_OPTIONS,
};

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual void SetInt(MaterialStandardParam param, int val) { };
	virtual void SetFloat(MaterialStandardParam param, float val) { };
	virtual void SetTexture(MaterialStandardParam param, ITexture* val) { };

	virtual ITexture* GetTexture(MaterialStandardParam param) { return NULL; };

	virtual void PreRender(IMaterial* prev_material) = 0;
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj) = 0;
	virtual void PostRender() = 0;

	virtual bool IsBlended() = 0;


	void SetShader(IShader* shader) { this->shader = shader; };
	IShader* GetShader() { return shader; };

	void SetName(const std::string& name) { this->name = name; };
	std::string& GetName() { return name; };

private:
	std::string name;

protected:
	IShader* shader;
};


// for creating materials with specific shaders (MSTSStandardMaterial (msts_standard_shader))
class IMaterialFactory
{
public:
	virtual ~IMaterialFactory() = default;

	virtual IMaterial* CreateMaterial() = 0;

	virtual std::string GetSpecificShader() = 0;
};

class IMaterialManager
{
public:
	virtual ~IMaterialManager() = default;

	virtual IMaterial* CreateMaterial(const std::string& name, const std::string& shader) = 0;
	virtual IMaterial* GetMaterial(const std::string& name) = 0;
	virtual bool IsMaterialCreated(const std::string& name) = 0;

	virtual void DeleteMaterial(IMaterial* material) = 0;

	virtual int MaterialReferenceIncrement(IMaterial* material) = 0;
	virtual int MaterialReferenceDecrement(IMaterial* material) = 0;

	virtual void RegisterMaterialFactory(IMaterialFactory* factory) = 0;
	/*
	virtual IMaterial* CreateMaterial(const std::string& name) = 0;
	virtual IMaterial* GetMaterial(const std::string& name) = 0;
	virtual bool IsMaterialCreated(const std::string& name) = 0;

	virtual void DeleteMaterial(IMaterial* material) = 0;

	virtual int MaterialReferenceIncrement(IMaterial* material) = 0;
	virtual int MaterialReferenceDecrement(IMaterial* material) = 0;
	*/
};

#define DEFINE_MATERIAL_FACTORY(name, shader_name) \
class name##Factory : public IMaterialFactory \
{ \
public: \
	name##Factory() {}; \
	~name##Factory() override {}; \
	virtual IMaterial* CreateMaterial(); \
	virtual std::string GetSpecificShader() { return shader_name; }; \
};