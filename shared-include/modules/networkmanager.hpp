#pragma once

#include "global.hpp"

#include "module.hpp"

#include "network/server/networkserver.hpp"
#include "network/client/networkclient.hpp"

#include "network/web/httprequest.hpp"

#include "network/services/authserver.hpp"

#include <string>

#pragma comment(lib, "Ws2_32.lib")

enum NetworkState
{
	NETWORK_STATE_UNKNOWN, // no one
	NETWORK_STATE_IS_CLIENT, // only client
	NETWORK_STATE_IS_SERVER, // only server
	NETWORK_STATE_IS_SELF_HOSTED // client and server
};

class INetworkManager : public IModule
{
public:
	virtual ~INetworkManager() = default;

	virtual TSResult InitializeClient() = 0;
	virtual TSResult InitializeServer() = 0;

	virtual TSResult Shutdown() = 0;

	virtual bool GetIsReady() = 0;

	virtual INetworkServer* GetNetworkServer() = 0;
	virtual INetworkClient* GetNetworkClient() = 0;

	virtual IHTTPRequest* CreateHTTPRequest() = 0;

	virtual IAuthServer* GetAuthServer() = 0;

	virtual int GetLastError() = 0;
	virtual std::string GetLastErrorString() = 0;
	virtual std::string GetLastErrorString(int error) = 0;

	virtual std::string GetIPByHostname(const std::string& hostname) = 0;
};