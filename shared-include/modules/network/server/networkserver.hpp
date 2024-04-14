#pragma once

#include <vector>

#include "networkremoteclient.hpp"

class INetworkServer
{
public:
	virtual ~INetworkServer() = default;

	virtual std::vector<INetworkRemoteClient*>& GetClients() = 0;
};