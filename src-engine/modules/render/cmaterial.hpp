#pragma once

#include "modules/render/material.hpp"

#include <map>
#include <string>

class CMaterial : public IMaterial
{
public:
	CMaterial();
	~CMaterial() override;

	virtual void SetShader(IShader* shader);
	virtual IShader* GetShader();

	virtual void SetTexture(MaterialStandartParam param, ITexture* val);

	virtual void PreRender();
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj);
	virtual void PostRender();

	virtual bool IsBlended();

	virtual std::string& GetName();
	virtual void SetName(const std::string& name);

	int ref_count;
private:
	std::string name;
	IShader* shader;

	ITexture* albedo_texture;
};

class CMaterialManager : public IMaterialManager
{
public:
	CMaterialManager();
	~CMaterialManager() override;

	virtual IMaterial* CreateMaterial(const std::string& name);
	virtual IMaterial* GetMaterial(const std::string& name);
	virtual bool IsMaterialCreated(const std::string& name);

	virtual void DeleteMaterial(IMaterial* material);

	virtual int MaterialReferenceIncrement(IMaterial* material);
	virtual int MaterialReferenceDecrement(IMaterial* material);
	
private:
	std::map<std::string, IMaterial*> materials;
};

extern CMaterialManager* g_MaterialManager;