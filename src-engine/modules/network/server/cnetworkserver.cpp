#include "cnetworkserver.hpp"

#include "modules/clogmodule.hpp"
#include "modules/cnetworkmanager.hpp"

CNetworkServer::CNetworkServer() : server_thread_enabled(true), from_thread_to_main(256)
{
	socket = new CNetworkSocket();

	InitializeThread();
}

CNetworkServer::~CNetworkServer()
{
	delete socket;

	server_thread_enabled = false;

	server_thread.join();
}

INetworkSocket* CNetworkServer::GetSocket()
{
	return socket;
}

void CNetworkServer::SendPacket(INetworkPacket* packet)
{
	GetSocket()->SendPacket(packet);

	g_NetworkManager->GetPacketManager()->PushPacketToPull(packet);
}

std::vector<INetworkRemoteClient*>& CNetworkServer::GetClients()
{
	return clients;
}

void CNetworkServer::PushPacketToMain(CNetworkPacket* packet)
{
	if (!from_thread_to_main.try_push(packet))
		delete packet;
}

CNetworkPacket* CNetworkServer::PopPacketFromThread()
{
	CNetworkPacket** ptr_packet = from_thread_to_main.front();

	if (ptr_packet == NULL)
		return NULL;

	CNetworkPacket* packet = *ptr_packet;

	from_thread_to_main.pop();

	return packet;
}

void CNetworkServer::Update()
{
	auto packet_manager = g_NetworkManager->GetPacketManager();

	while (true)
	{
		CNetworkPacket* packet = PopPacketFromThread();
	
		if (packet == NULL) break;

		HandlePacket(packet);

		packet_manager->PushPacketToPull(packet);
	}
}

void CNetworkServer::HandlePacket(CNetworkPacket* packet)
{
	NetworkPacketID id = packet->GetID();

	switch (id)
	{
	case PACKET_TO_SERVER_INFO_REQUEST:
		break;
	case PACKET_TO_SERVER_CONNECT_REQUEST:
		g_Log->LogInfo("Username: %s", packet->ReadString().c_str());
		break;
	case PACKET_TO_SERVER_DISCONNECT:
		break;
	case PACKET_TO_SERVER_PONG:
		break;
	default:
		break;
	}

}

void CNetworkServer::InitializeThread()
{
	server_thread = std::thread(ServerThread, this);
}

void CNetworkServer::ServerThread(CNetworkServer* server)
{
	NetworkIPAddress sender;
	
	auto socket = server->GetSocket();
	auto packet_manager = g_NetworkManager->GetPacketManager();

	while (server->server_thread_enabled.load())
	{
		INetworkPacket* packet = packet_manager->PopPacketFromPull();

		TSResult result = socket->ReceivePacket(packet);

		if (result != TS_OK)
		{
			packet_manager->PushPacketToPull(packet);

			int error = g_NetworkManager->GetLastError();

			if (error == WSAEINTR)
				break;

			if (error == WSAEINVAL)
				continue;

			g_Log->LogError("ServerThread Receive failed: (%d) %s", error, g_NetworkManager->GetLastErrorString(error).c_str());
			break;
		}

		if (result == TS_OK)
			server->PushPacketToMain((CNetworkPacket*)packet);
	}
}