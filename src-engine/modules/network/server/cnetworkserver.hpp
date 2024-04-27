#pragma once

#include "modules/network/server/networkserver.hpp"

#include "modules/network/cnetworksocket.hpp"
#include "modules/network/cnetworkpacket.hpp"

#include "rigtorp/SPSCQueue.h"

#include <thread>
#include <atomic>

class CNetworkServer : public INetworkServer
{
public:
	CNetworkServer();
	~CNetworkServer() override;

	virtual INetworkSocket* GetSocket();
	virtual void SendPacket(INetworkPacket* packet);

	virtual std::vector<INetworkRemoteClient*>& GetClients();

	void Update();
private:

	void PushPacketToMain(CNetworkPacket* packet);
	CNetworkPacket* PopPacketFromThread();
	void HandlePacket(CNetworkPacket* packet);

	void InitializeThread();

	static void ServerThread(CNetworkServer* server);

	std::thread server_thread;
	std::atomic<bool> server_thread_enabled;

	rigtorp::SPSCQueue<CNetworkPacket*> from_thread_to_main;

	CNetworkSocket* socket;

	std::vector<INetworkRemoteClient*> clients;
};