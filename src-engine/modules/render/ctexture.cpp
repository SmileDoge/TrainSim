#include "ctexture.hpp"
#include "glad/glad.h"

#include "modules/crendermodule.hpp"
#include "modules/clogmodule.hpp"

CTexture::CTexture() : ref_count(0), width(0), height(0), format(TEXTURE_FORMAT_RGB), name("CTexture")
{
	glGenTextures(1, &texture_handle);
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &texture_handle);
}

void CTexture::Use(TextureUnit unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
}

int GetOpenGLWrap(TextureWrap wrap)
{
	switch (wrap)
	{
	case TEXTURE_WRAP_REPEAT:
		return GL_REPEAT;
	case TEXTURE_WRAP_MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case TEXTURE_WRAP_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case TEXTURE_WRAP_CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	default:
		return GL_REPEAT;
	}
}

int GetOpenGLFilter(TextureFilter filter)
{
	switch (filter)
	{
	case TEXTURE_FILTER_LINEAR:
		return GL_LINEAR;
	case TEXTURE_FILTER_NEAREST:
		return GL_NEAREST;
	default:
		return GL_LINEAR;
	}
}

int GetOpenGLInternalFormat(TexturePixelFormat format)
{
	switch (format)
	{
	case TEXTURE_FORMAT_RGBA:
		return GL_RGBA;
	case TEXTURE_FORMAT_RGB:
		return GL_RGB;
	case TEXTURE_FORMAT_BGRA:
		return GL_RGBA;
	case TEXTURE_FORMAT_BGR:
		return GL_RGB;
	case TEXTURE_FORMAT_RGB565:
		return GL_RGB;
	case TEXTURE_FORMAT_RGBA5551:
		return GL_RGBA;
	case TEXTURE_FORMAT_BGR565:
		return GL_RGB;
	case TEXTURE_FORMAT_BGRA5551:
		return GL_RGBA;
	case TEXTURE_FORMAT_BGRA4444:
		return GL_RGBA;
	case TEXTURE_FORMAT_RED:
		return GL_RED;
	case TEXTURE_FORMAT_DXT1:
		return GL_RGB;
	case TEXTURE_FORMAT_DXT3:
		return GL_RGBA;
	case TEXTURE_FORMAT_DXT5:
		return GL_RGBA;
	default:
		return GL_RGB;
	}
}

int GetOpenGLFormat(TexturePixelFormat format)
{
	switch (format)
	{
	case TEXTURE_FORMAT_RGBA:
		return GL_RGBA;
	case TEXTURE_FORMAT_RGB:
		return GL_RGB;
	case TEXTURE_FORMAT_BGRA:
		return GL_BGRA;
	case TEXTURE_FORMAT_BGR:
		return GL_BGR;
	case TEXTURE_FORMAT_RGB565:
		return GL_RGB;
	case TEXTURE_FORMAT_RGBA5551:
		return GL_RGBA;
	case TEXTURE_FORMAT_BGR565:
		return GL_BGR;
	case TEXTURE_FORMAT_BGRA5551:
		return GL_BGRA;
	case TEXTURE_FORMAT_BGRA4444:
		return GL_BGRA;
	case TEXTURE_FORMAT_RED:
		return GL_RED;
	case TEXTURE_FORMAT_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case TEXTURE_FORMAT_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	default:
		return GL_RGB;
	}
}

int GetOpenGLDataType(TexturePixelFormat format)
{
	switch (format)
	{
	case TEXTURE_FORMAT_RGBA:
		return GL_UNSIGNED_BYTE;
	case TEXTURE_FORMAT_RGB:
		return GL_UNSIGNED_BYTE;
	case TEXTURE_FORMAT_BGRA:
		return GL_UNSIGNED_BYTE;
	case TEXTURE_FORMAT_BGR:
		return GL_UNSIGNED_BYTE;
	case TEXTURE_FORMAT_RGB565:
		return GL_UNSIGNED_SHORT_5_6_5;
	case TEXTURE_FORMAT_RGBA5551:
		return GL_UNSIGNED_SHORT_5_5_5_1;
	case TEXTURE_FORMAT_BGR565:
		return GL_UNSIGNED_SHORT_5_6_5;
	case TEXTURE_FORMAT_BGRA5551:
		return GL_UNSIGNED_SHORT_5_5_5_1;
	case TEXTURE_FORMAT_BGRA4444:
		return GL_UNSIGNED_SHORT_4_4_4_4;
	case TEXTURE_FORMAT_RED:
		return GL_UNSIGNED_BYTE;
	default:
		return GL_UNSIGNED_BYTE;
	}
}

bool IsCompressedFormat(TexturePixelFormat format)
{
	if (format >= TEXTURE_FORMAT_COMPRESSED_FORMATS)
		return true;

	return false;
}

void CTexture::SetWrap(TextureWrap wrap_s, TextureWrap wrap_t)
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrap(wrap_s));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrap(wrap_t));
}

void CTexture::SetFilter(TextureFilter min, TextureFilter mag)
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetOpenGLFilter(min));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetOpenGLFilter(mag));
}

void CTexture::SetData(int width, int height, TexturePixelFormat format, void* data)
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	this->width = width;
	this->height = height;
	this->format = format;

	int gl_int_format = GetOpenGLInternalFormat(format);
	int gl_format = GetOpenGLFormat(format);
	int gl_data = GetOpenGLDataType(format);

	bool is_compressed = IsCompressedFormat(format);

	if (is_compressed)
	{
		g_Log->LogError("CTexture::SetData(%d) - Texture format is compressed!", format);
		__debugbreak();
	}

	glTexImage2D(GL_TEXTURE_2D, 0, gl_int_format, width, height, 0, gl_format, gl_data, data);
}

void CTexture::SetCompressedData(int width, int height, TexturePixelFormat format, void* data, size_t data_len)
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	this->width = width;
	this->height = height;
	this->format = format;

	int gl_format = GetOpenGLFormat(format);

	bool is_compressed = IsCompressedFormat(format);

	if (!is_compressed)
	{
		g_Log->LogError("CTexture::SetCompressedData(%d) - Texture format is not compressed!", format);
		__debugbreak();
	}

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, data_len, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, gl_int_format, width, height, 0, gl_format, gl_data, data);
}

void CTexture::GenerateMipmap()
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	glGenerateMipmap(GL_TEXTURE_2D);
}

int CTexture::GetWidth()
{
	return width;
}

int CTexture::GetHeight()
{
	return height;
}

TexturePixelFormat CTexture::GetFormat()
{
	return format;
}

void CTexture::SetName(const std::string& name)
{
	this->name = name;

	glBindTexture(GL_TEXTURE_2D, texture_handle);

	if (g_Render->ExtensionCanLabel())
		glObjectLabel(GL_TEXTURE, texture_handle, -1, name.c_str());
}

std::string& CTexture::GetName()
{
	return name;
}

CTextureManager* g_TextureManager = NULL;

CTextureManager::CTextureManager() : textures()
{
	g_TextureManager = this;
}

CTextureManager::~CTextureManager()
{
	for (auto& [name, texture] : textures)
		delete texture;
}


ITexture* CTextureManager::CreateTexture(const std::string& name)
{
	if (textures.find(name) != textures.end())
		return textures[name];

	CTexture* texture = new CTexture();

	texture->SetName(name);

	textures[name] = texture;

	return texture;
}

ITexture* CTextureManager::GetTexture(const std::string& name)
{
	if (textures.find(name) != textures.end())
		return textures[name];

	return NULL;
}

bool CTextureManager::IsTextureCreated(const std::string& name)
{
	if (textures.find(name) == textures.end())
		return false;

	return true;
}

void CTextureManager::DeleteTexture(ITexture* texture)
{
	if (textures.find(texture->GetName()) == textures.end())
		return;

	textures.erase(texture->GetName());

	delete texture;
}

int CTextureManager::TextureReferenceIncrement(ITexture* texture)
{
	return ++((CTexture*)texture)->ref_count;
}

int CTextureManager::TextureReferenceDecrement(ITexture* texture)
{
	int new_count = --((CTexture*)texture)->ref_count;

	if (new_count <= 0)
		DeleteTexture(texture);

	return new_count;
}