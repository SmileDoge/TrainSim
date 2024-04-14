#pragma once

#include "modules/network/networksocket.hpp"

class INetworkClient
{
public:
	virtual ~INetworkClient() = default;

	virtual INetworkSocket* GetSocket() = 0;
};