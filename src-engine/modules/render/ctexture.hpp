#pragma once

#include "modules/render/texture.hpp"

#include <map>

class CTexture : public ITexture
{
public:
	CTexture();
	~CTexture() override;

	virtual void Use(TextureUnit unit = TEXTURE_UNIT_0);

	virtual void SetWrap(TextureWrap wrap_s, TextureWrap wrap_t);
	virtual void SetFilter(TextureFilter min, TextureFilter mag);
	virtual void SetData(int width, int height, TexturePixelFormat format, void* data);

	virtual void GenerateMipmap();

	virtual int GetWidth();
	virtual int GetHeight();
	virtual TexturePixelFormat GetFormat();

	virtual std::string& GetName();
	virtual void SetName(const std::string& name);

	virtual unsigned int GetID() { return texture_handle; };

	int ref_count;

private:
	int width, height;
	TexturePixelFormat format;

	std::string name;

	unsigned int texture_handle;
};


class CTextureManager : public ITextureManager
{
public:
	CTextureManager();
	~CTextureManager() override;

	virtual ITexture* CreateTexture(const std::string& name);
	virtual ITexture* GetTexture(const std::string& name);
	virtual bool IsTextureCreated(const std::string& name);

	virtual void DeleteTexture(ITexture* texture);

	virtual int TextureReferenceIncrement(ITexture* texture);
	virtual int TextureReferenceDecrement(ITexture* texture);

private:
	std::map<std::string, ITexture*> textures;
};

extern CTextureManager* g_TextureManager;