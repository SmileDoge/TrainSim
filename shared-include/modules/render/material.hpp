#pragma once

#include <string>
#include <vector>
#include "shader.hpp"
#include "texture.hpp"
#include "renderitem.hpp"

enum MaterialStandartParam
{
	MATERIAL_ALBEDO_TEXTURE,
};

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual void SetShader(IShader* shader) = 0;
	virtual IShader* GetShader() = 0;

	virtual void SetInt(MaterialStandartParam param, int val) { };
	virtual void SetFloat(MaterialStandartParam param, float val) { };
	virtual void SetTexture(MaterialStandartParam param, ITexture* val) { };

	virtual ITexture* GetTexture(MaterialStandartParam param) { return NULL; };

	virtual void PreRender() = 0;
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj) = 0;
	virtual void PostRender() = 0;

	virtual bool IsBlended() = 0;

	virtual std::string& GetName() = 0;
	virtual void SetName(const std::string& name) = 0;
};

class IMaterialManager
{
public:
	virtual ~IMaterialManager() = default;

	virtual IMaterial* CreateMaterial(const std::string& name) = 0;
	virtual IMaterial* GetMaterial(const std::string& name) = 0;
	virtual bool IsMaterialCreated(const std::string& name) = 0;

	virtual void DeleteMaterial(IMaterial* material) = 0;

	virtual int MaterialReferenceIncrement(IMaterial* material) = 0;
	virtual int MaterialReferenceDecrement(IMaterial* material) = 0;
};