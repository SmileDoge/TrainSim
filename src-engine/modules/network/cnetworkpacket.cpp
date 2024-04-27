#include "cnetworkpacket.hpp"

CNetworkPacketManager::CNetworkPacketManager() : packets(1024)
{
}

CNetworkPacketManager::~CNetworkPacketManager()
{
}

INetworkPacket* CNetworkPacketManager::PopPacketFromPull()
{
	CNetworkPacket* packet = NULL;

	if (!packets.try_pop(packet))
		packet = new CNetworkPacket();

	return packet;
}

void CNetworkPacketManager::PushPacketToPull(INetworkPacket* packet)
{
	CNetworkPacket* c_packet = (CNetworkPacket*)packet;

	if (!packets.try_push(c_packet))
		delete c_packet;
}

CNetworkPacket::CNetworkPacket() : offset(0), size(0)
{
	buffer = new uint8_t[PACKET_DATA_SIZE];

	memset(buffer, 0, PACKET_DATA_SIZE);
}

CNetworkPacket::~CNetworkPacket()
{
	delete[] buffer;
}

NetworkPacketID CNetworkPacket::GetID()
{
	if (offset == 0) offset = 1;

	return (NetworkPacketID)buffer[0];
}

void CNetworkPacket::SetID(NetworkPacketID id)
{
	buffer[0] = id;
}

NetworkIPAddress CNetworkPacket::GetAddress()
{
	return address;
}

void CNetworkPacket::SetAddress(NetworkIPAddress& address)
{
	this->address = address;
}

unsigned char* CNetworkPacket::GetData()
{
	return buffer;
}

void* CNetworkPacket::GetPointer()
{
	return ((char*)buffer) + offset;
}

size_t CNetworkPacket::Size()
{
	return size;
}

size_t CNetworkPacket::Tell()
{
	return offset;
}

void CNetworkPacket::Seek(size_t pos)
{
	offset = pos;
}

void CNetworkPacket::InitializeReadPacket(uint8_t* another_buffer, size_t size)
{
	memcpy(buffer, another_buffer, size);

	this->size = size;
	this->offset = 0;
}

void CNetworkPacket::InitializeWritePacket()
{
	this->size = PACKET_DATA_SIZE;
	this->offset = 0;
}

#define WRITE_FUNCTION(name, type) \
const int __##name##_size = sizeof(type); \
void CNetworkPacket::Write##name(type val) \
{ \
	if ((PACKET_DATA_SIZE - offset - 1) < __##name##_size) return; \
	memcpy(buffer + offset, &val, __##name##_size); \
	offset += __##name##_size; \
	size = offset; \
} \

#define READ_FUNCTION(name, type) \
type CNetworkPacket::Read##name() \
{ \
	if ((size - offset - 1) < __##name##_size) return 0; \
	type result = 0; \
	memcpy(&result, buffer + offset, __##name##_size); \
	offset += __##name##_size; \
	return result; \
} \

WRITE_FUNCTION(UInt8, uint8_t)
WRITE_FUNCTION(UInt16, uint16_t)
WRITE_FUNCTION(UInt32, uint32_t)

WRITE_FUNCTION(Int8, int8_t)
WRITE_FUNCTION(Int16, int16_t)
WRITE_FUNCTION(Int32, int32_t)

WRITE_FUNCTION(Float, float)
WRITE_FUNCTION(Double, double)


READ_FUNCTION(UInt8, uint8_t)
READ_FUNCTION(UInt16, uint16_t)
READ_FUNCTION(UInt32, uint32_t)
READ_FUNCTION(Int8, int8_t)
READ_FUNCTION(Int16, int16_t)
READ_FUNCTION(Int32, int32_t)

READ_FUNCTION(Float, float)
READ_FUNCTION(Double, double)

void CNetworkPacket::WriteBytes(uint8_t* src, size_t len)
{
	if ((PACKET_DATA_SIZE - offset - 1) < len) return;
	memcpy(buffer + offset, src, len);
	offset += len;
	size = offset;
}

void CNetworkPacket::WriteString(const std::string& str)
{
	int size = str.size();

	WriteUInt16(size);

	WriteBytes((uint8_t*)str.data(), str.size());
}

void CNetworkPacket::ReadBytes(uint8_t* dest, size_t len)
{
	if ((size - offset - 1) < len) return;
	memcpy(dest, buffer + offset, len);
	offset += len;
}

std::string CNetworkPacket::ReadString()
{
	uint16_t len = ReadUInt16();

	std::string str = std::string((char*)GetPointer(), len);

	Seek(Tell() + len);

	return str;
}
