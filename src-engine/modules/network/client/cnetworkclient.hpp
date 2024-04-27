#pragma once

#include "modules/network/client/networkclient.hpp"

#include "modules/network/cnetworksocket.hpp"
#include "modules/network/cnetworkpacket.hpp"

#include "rigtorp/SPSCQueue.h"

#include <thread>
#include <atomic>
class CNetworkClient : public INetworkClient
{
public:
	CNetworkClient();
	~CNetworkClient() override;

	virtual void Connect(const std::string& host, uint16_t port = 23000);

	virtual INetworkSocket* GetSocket();
	virtual void SendPacket(INetworkPacket* packet);

	void Update();
private:

	void PushPacketToMain(CNetworkPacket* packet);
	CNetworkPacket* PopPacketFromThread();
	void HandlePacket(CNetworkPacket* packet);

	void InitializeThread();

	static void ClientThread(CNetworkClient* client);

	std::thread client_thread;
	std::atomic<bool> client_thread_enabled;

	rigtorp::SPSCQueue<CNetworkPacket*> from_thread_to_main;

	CNetworkSocket* socket;
};