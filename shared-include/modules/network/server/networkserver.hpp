#pragma once

#include <vector>

#include "networkremoteclient.hpp"
#include "modules/network/networksocket.hpp"

#include "modules/network/networkpacket.hpp"

class INetworkServerPacketHandler;

class INetworkServer
{
public:
	virtual ~INetworkServer() = default;

	virtual INetworkSocket* GetSocket() = 0;
	virtual void SendPacket(INetworkPacket* packet) = 0;

	virtual std::vector<INetworkRemoteClient*>& GetClients() = 0;

	//virtual INetworkServerPacketHandler* GetPacketHandler() = 0;
	//virtual void SetPacketHandler(INetworkServerPacketHandler* handler) = 0;
};

class INetworkServerPacketHandler
{
public:
	virtual ~INetworkServerPacketHandler() = default;

	virtual TSResult HandlePacket(INetworkPacket* packet, INetworkServer* server) = 0;
};