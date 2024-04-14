#pragma once

typedef unsigned short USHORT;

enum TSResult {
	TS_OK,

	TS_ERROR,
	TS_NOT_FOUND,
	TS_INVALID_ARGS,
	TS_INVALID_OPERATION,
	TS_OUT_OF_MEMORY,

	TS_ALREADY_USED,
};

typedef struct TSVersion_t
{
	short major, minor, patch;

	TSVersion_t()
	{
		major = 0;
		minor = 0;
		patch = 0;
	}

	TSVersion_t(short major, short minor, short patch)
	{
		this->major = major;
		this->minor = minor;
		this->patch = patch;
	}

	TSVersion_t(short major, short minor) : TSVersion_t(major, minor, 0)
	{

	}
} TSVersion;