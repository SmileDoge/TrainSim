#pragma once

#include "modules/networkmanager.hpp"

#include "network/cnetworkpacket.hpp"

#include "network/web/chttprequest.hpp"

#include <thread>
#include <atomic>

#include "rigtorp/SPSCQueue.h"

class CNetworkServer;
class CNetworkClient;
class CAuthServer;

class CNetworkManager : public INetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager() override;

	virtual TSResult InitializeClient();
	virtual TSResult InitializeServer();
	virtual TSResult InitializeSelfHost();

	virtual TSResult Shutdown();

	virtual bool GetIsReady();

	virtual INetworkServer* GetNetworkServer();
	virtual INetworkClient* GetNetworkClient();

	virtual IHTTPRequest* CreateHTTPRequest();

	virtual INetworkPacket* CreatePacket(NetworkPacketID id);
	virtual INetworkPacketManager* GetPacketManager();

	virtual IAuthServer* GetAuthServer();

	virtual int GetLastError();
	virtual std::string GetLastErrorString();
	virtual std::string GetLastErrorString(int error);

	virtual std::string GetIPByHostname(const std::string& hostname);
	virtual NetworkIPAddress GetNetworkAddressByHostname(const std::string& hostname);

	virtual void Update();

	void AddHTTPRequestToQueue(CHTTPRequest* request);

private:

	static void HTTPRequestHandler();

	rigtorp::SPSCQueue<CHTTPRequest*> requests_to_thread, requests_from_thread;
	std::thread request_handler;
	std::atomic<bool> request_handler_enable;

	NetworkState state;

	bool initialized;

	CNetworkServer* server;
	CNetworkClient* client;

	CAuthServer* auth_server;

	CNetworkPacketManager* packet_manager;
};

extern CNetworkManager* g_NetworkManager;