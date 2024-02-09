#pragma once

enum TS_Texture_Format
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

struct TS_Texture_Data
{
	int width;
	int height;
	TS_Texture_Format format;
	int size_data;
	int ptr_data;
};