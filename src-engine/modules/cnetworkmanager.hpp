#pragma once

#include "modules/networkmanager.hpp"

#include "network/web/chttprequest.hpp"

#include <thread>

#include "rigtorp/SPSCQueue.h"

class CNetworkManager : public INetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager() override;

	virtual TSResult InitializeClient();
	virtual TSResult InitializeServer();

	virtual TSResult Shutdown();

	virtual bool GetIsReady();

	virtual INetworkServer* GetNetworkServer();
	virtual INetworkClient* GetNetworkClient();

	virtual IHTTPRequest* CreateHTTPRequest();

	virtual IAuthServer* GetAuthServer();

	virtual int GetLastError();
	virtual std::string GetLastErrorString();
	virtual std::string GetLastErrorString(int error);

	virtual std::string GetIPByHostname(const std::string& hostname);

	virtual void Update();

	void AddHTTPRequestToQueue(CHTTPRequest* request);

private:

	static void HTTPRequestHandler();

	rigtorp::SPSCQueue<CHTTPRequest*> requests_to_thread, requests_from_thread;
	std::thread request_handler;

	NetworkState state;

	bool initialized;

	INetworkServer* server;
	INetworkClient* client;

	IAuthServer* auth_server;
};

extern CNetworkManager* g_NetworkManager;