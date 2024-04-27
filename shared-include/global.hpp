#pragma once

#include "stdint.h"

typedef unsigned short USHORT;

struct NetworkIPAddress
{
	union
	{
		struct { uint8_t first, second, third, fourth; } octets;
		uint32_t full;
	} address;
	uint16_t port;

	NetworkIPAddress()
	{
		address.full = 0x00000000;
		port = 23000;
	}

	NetworkIPAddress(uint16_t port)
	{
		address.full = 0x00000000;
		this->port = port;
	}

	NetworkIPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint16_t port)
	{
		address.octets.first = first;
		address.octets.second = second;
		address.octets.third = third;
		address.octets.fourth = fourth;
		this->port = port;
	}

	NetworkIPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
	{
		address.octets.first = first;
		address.octets.second = second;
		address.octets.third = third;
		address.octets.fourth = fourth;
		port = 23000;
	}
};

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