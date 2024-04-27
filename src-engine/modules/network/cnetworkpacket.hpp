#pragma once

#include "modules/network/networkpacket.hpp"

#include "rigtorp/MPMCQueue.h"

class CNetworkPacket : public INetworkPacket
{
public:
	CNetworkPacket();
	~CNetworkPacket() override;

	virtual NetworkPacketID GetID();
	virtual void SetID(NetworkPacketID id);

	virtual NetworkIPAddress GetAddress();
	virtual void SetAddress(NetworkIPAddress& address);


	virtual void WriteUInt8(uint8_t val);
	virtual void WriteUInt16(uint16_t val);
	virtual void WriteUInt32(uint32_t val);

	virtual void WriteInt8(int8_t val);
	virtual void WriteInt16(int16_t val);
	virtual void WriteInt32(int32_t val);

	virtual void WriteBytes(uint8_t* src, size_t len);
	virtual void WriteString(const std::string& str) override;
	virtual void WriteFloat(float val);
	virtual void WriteDouble(double val);


	virtual uint8_t ReadUInt8();
	virtual uint16_t ReadUInt16();
	virtual uint32_t ReadUInt32();

	virtual int8_t ReadInt8();
	virtual int16_t ReadInt16();
	virtual int32_t ReadInt32();

	virtual void ReadBytes(uint8_t* dest, size_t len);
	virtual std::string ReadString();
	virtual float ReadFloat();
	virtual double ReadDouble();


	virtual unsigned char* GetData();
	virtual void* GetPointer();

	virtual size_t Size();
	virtual size_t Tell();
	virtual void Seek(size_t pos);

	void InitializeReadPacket(uint8_t* another_buffer, size_t size);
	void InitializeWritePacket();
private:
	uint8_t* buffer;
	size_t offset;
	size_t size;

	NetworkIPAddress address;
};

class CNetworkPacketManager : public INetworkPacketManager
{
public:
	CNetworkPacketManager();
	~CNetworkPacketManager() override;

	virtual INetworkPacket* PopPacketFromPull();
	virtual void PushPacketToPull(INetworkPacket* packet);

private:
	rigtorp::MPMCQueue<CNetworkPacket*> packets;
};