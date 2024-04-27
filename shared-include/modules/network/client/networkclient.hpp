#pragma once

#include "modules/network/networksocket.hpp"

class INetworkClient
{
public:
	virtual ~INetworkClient() = default;

	virtual void Connect(const std::string& host, uint16_t port = 23000) = 0;

	virtual INetworkSocket* GetSocket() = 0;
	virtual void SendPacket(INetworkPacket* packet) = 0;
};