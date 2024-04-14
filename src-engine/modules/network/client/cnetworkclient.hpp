#pragma once

#include "modules/network/client/networkclient.hpp"

class CNetworkClient : public INetworkClient
{
public:
	CNetworkClient();
	~CNetworkClient() override;

	virtual INetworkSocket* GetSocket();
};