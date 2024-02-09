#pragma once

#include <string>

enum TexturePixelFormat
{
	TEXTURE_FORMAT_RGBA,
	TEXTURE_FORMAT_RGB,
	TEXTURE_FORMAT_BGRA,
	TEXTURE_FORMAT_BGR,
	TEXTURE_FORMAT_RGB565,
	TEXTURE_FORMAT_RGBA5551,
	TEXTURE_FORMAT_BGR565,
	TEXTURE_FORMAT_BGRA5551,
	TEXTURE_FORMAT_BGRA4444,
	TEXTURE_FORMAT_RED,
	TEXTURE_FORMAT_DXT1,
	TEXTURE_FORMAT_DXT3,
	TEXTURE_FORMAT_DXT5,
};

enum TextureWrap
{
	TEXTURE_WRAP_REPEAT,
	TEXTURE_WRAP_MIRRORED_REPEAT,
	TEXTURE_WRAP_CLAMP_TO_EDGE,
	TEXTURE_WRAP_CLAMP_TO_BORDER,
};

enum TextureFilter
{
	TEXTURE_FILTER_LINEAR,
	TEXTURE_FILTER_NEAREST,
};

enum TextureUnit
{
	TEXTURE_UNIT_0,
	TEXTURE_UNIT_1,
	TEXTURE_UNIT_2,
	TEXTURE_UNIT_3,
	TEXTURE_UNIT_4,
	TEXTURE_UNIT_5,
	TEXTURE_UNIT_6,
	TEXTURE_UNIT_7,
	TEXTURE_UNIT_8,
	TEXTURE_UNIT_9,
	TEXTURE_UNIT_10,
	TEXTURE_UNIT_11,
	TEXTURE_UNIT_12,
	TEXTURE_UNIT_13,
	TEXTURE_UNIT_14,
	TEXTURE_UNIT_15,
};

class ITexture
{
public:
	virtual ~ITexture() = default;

	virtual void Use(TextureUnit unit = TEXTURE_UNIT_0) = 0;

	virtual void SetWrap(TextureWrap wrap_s, TextureWrap wrap_t) = 0;
	virtual void SetFilter(TextureFilter min, TextureFilter mag) = 0;
	virtual void SetData(int width, int height, TexturePixelFormat format, void* data) = 0;

	virtual void GenerateMipmap() = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual TexturePixelFormat GetFormat() = 0;

	virtual std::string& GetName() = 0;
	virtual void SetName(const std::string& name) = 0;

	virtual unsigned int GetID() = 0;
};

class ITextureManager
{
public:
	virtual ~ITextureManager() = default;

	virtual ITexture* CreateTexture(const std::string& name) = 0;
	virtual ITexture* GetTexture(const std::string& name) = 0;
	virtual bool IsTextureCreated(const std::string& name) = 0;

	virtual void DeleteTexture(ITexture* texture) = 0;

	virtual int TextureReferenceIncrement(ITexture* texture) = 0;
	virtual int TextureReferenceDecrement(ITexture* texture) = 0;
};