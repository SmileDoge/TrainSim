#pragma once

#include "global.hpp"

#include <string>

enum NetworkPacketID : unsigned char
{
	// C2S

	PACKET_TO_SERVER_INFO_REQUEST = 0,

	PACKET_TO_SERVER_CONNECT_REQUEST,
	PACKET_TO_SERVER_DISCONNECT,

	PACKET_TO_SERVER_PONG,

	// S2C

	PACKET_TO_CLIENT_INFO_RESPONSE = 0,

	PACKET_TO_CLIENT_CONNECT_RESPONSE,
	PACKET_TO_CLIENT_DISCONNECT,

	PACKET_TO_CLIENT_PING,
};

#define PACKET_DATA_SIZE 65536

class INetworkPacket
{
public:
	virtual ~INetworkPacket() = default;

	virtual NetworkPacketID GetID() = 0;
	virtual void SetID(NetworkPacketID id) = 0;

	virtual NetworkIPAddress GetAddress() = 0;
	virtual void SetAddress(NetworkIPAddress& address) = 0;


	virtual void WriteUInt8(uint8_t val) = 0;
	virtual void WriteUInt16(uint16_t val) = 0;
	virtual void WriteUInt32(uint32_t val) = 0;

	virtual void WriteInt8(int8_t val) = 0;
	virtual void WriteInt16(int16_t val) = 0;
	virtual void WriteInt32(int32_t val) = 0;

	virtual void WriteBytes(uint8_t* src, size_t len) = 0;
	virtual void WriteString(const std::string& str) = 0;
	virtual void WriteFloat(float val) = 0;
	virtual void WriteDouble(double val) = 0;


	virtual uint8_t ReadUInt8() = 0;
	virtual uint16_t ReadUInt16() = 0;
	virtual uint32_t ReadUInt32() = 0;

	virtual int8_t ReadInt8() = 0;
	virtual int16_t ReadInt16() = 0;
	virtual int32_t ReadInt32() = 0;

	virtual void ReadBytes(uint8_t* dest, size_t len) = 0;
	virtual std::string ReadString() = 0;
	virtual float ReadFloat() = 0;
	virtual double ReadDouble() = 0;


	virtual uint8_t* GetData() = 0;
	virtual void* GetPointer() = 0;

	virtual size_t Size() = 0;
	virtual size_t Tell() = 0;
	virtual void Seek(size_t pos) = 0;
};

class INetworkPacketManager
{
public:
	virtual ~INetworkPacketManager() = default;

	virtual INetworkPacket* PopPacketFromPull() = 0;
	virtual void PushPacketToPull(INetworkPacket* packet) = 0;
};