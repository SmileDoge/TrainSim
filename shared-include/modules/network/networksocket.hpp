#pragma once

#include <string>

#include "global.hpp"

#include "modules/network/networkpacket.hpp"

class INetworkSocket
{
public:
	virtual ~INetworkSocket() = default;

	virtual TSResult Bind(unsigned short port) = 0;
	virtual TSResult BindWithIP(const std::string& ip, unsigned short port) = 0;

	virtual size_t SendBytes(unsigned char* bytes, size_t bytes_len, NetworkIPAddress& receiver) = 0;
	virtual size_t ReceiveBytes(unsigned char* buffer, size_t buffer_len, NetworkIPAddress& sender) = 0;

	virtual TSResult SendPacket(INetworkPacket* packet) = 0;
	virtual TSResult ReceivePacket(INetworkPacket* packet) = 0;
};