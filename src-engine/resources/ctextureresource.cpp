#include "ctextureresource.hpp"

#include "modules/clogmodule.hpp"
#include "modules/crendermodule.hpp"

#include "global.hpp"

#include <string>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

TextureResourceFactory::TextureResourceFactory()
{
}

TextureResourceFactory::~TextureResourceFactory()
{
}

TSResult TextureResourceFactory::CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options)
{
	CTSTextureResource* res_texture = new CTSTextureResource();

	TSResult result = res_texture->LoadFromStream(stream, (TextureResourceLoadOptions*)(void*)options);

	resource = res_texture;

	return result;
}

CTSTextureResource::CTSTextureResource()
{
	SetType(RESOURCE_TYPE_TEXTURE);
}

CTSTextureResource::~CTSTextureResource()
{
}

ITexture* CTSTextureResource::GetData()
{
	return texture;
}

enum TSTexture_format
{
	RGBA32,
	BGRA32,
	BGR565,
	BGRA5551,
	BGRA4444,
	DXT1,
	DXT3,
	DXT5,
};

#include <filesystem>

TexturePixelFormat GetPixelFormat(TSTexture_format format)
{
	switch (format)	
	{
	case RGBA32:
		return TEXTURE_FORMAT_RGBA;
		break;
	case BGRA32:
		return TEXTURE_FORMAT_BGRA;
		break;
	case BGR565:
		return TEXTURE_FORMAT_BGR565;
		break;
	case BGRA5551:
		return TEXTURE_FORMAT_BGRA5551;
		break;
	case BGRA4444:
		return TEXTURE_FORMAT_BGRA4444;
		break;
	case DXT1:
		return TEXTURE_FORMAT_DXT1;
		break;
	case DXT3:
		return TEXTURE_FORMAT_DXT3;
		break;
	case DXT5:
		return TEXTURE_FORMAT_DXT5;
		break;
	default:
		g_Log->LogError("Unknown pixel format for texture: %d", format);
		return TEXTURE_FORMAT_RGBA;
		break;
	}
}

TSResult CTSTextureResource::LoadFromStream(IFileStream* stream, TextureResourceLoadOptions* options)
{
	int width, height, data_len;
	TexturePixelFormat format;

	width = stream->ReadInt32();
	height = stream->ReadInt32();
	format = GetPixelFormat((TSTexture_format)stream->ReadInt32());
	data_len = stream->ReadInt32();

	char* data = new char[data_len];

	size_t readed_len = stream->ReadBytes(data, data_len);

	if (data_len != readed_len)
	{
		return TS_INVALID_OPERATION;
		delete[] data;
	}

	texture = g_Render->GetTextureManager()->CreateTexture(stream->GetPath());

	texture->SetFilter(TEXTURE_FILTER_LINEAR, TEXTURE_FILTER_LINEAR);
	texture->SetWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
	
	if (ITexture::IsCompressedFormat(format))
		texture->SetCompressedData(width, height, format, data, data_len);
	else
		texture->SetData(width, height, format, data);
	
	texture->GenerateMipmap();

	delete[] data;

	return TS_OK;
}
