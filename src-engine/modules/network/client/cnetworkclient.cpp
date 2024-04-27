#include "cnetworkclient.hpp"

#include "modules/cnetworkmanager.hpp"
#include "modules/clogmodule.hpp"

CNetworkClient::CNetworkClient() : client_thread_enabled(true), from_thread_to_main(256)
{
	socket = new CNetworkSocket();

	InitializeThread();
}

CNetworkClient::~CNetworkClient()
{
	delete socket;

	client_thread_enabled = false;

	client_thread.join();
}

void CNetworkClient::Connect(const std::string& host, uint16_t port)
{
	auto server = g_NetworkManager->GetNetworkAddressByHostname(host);

	server.port = port;

	auto packet = g_NetworkManager->CreatePacket(PACKET_TO_SERVER_CONNECT_REQUEST);

	packet->SetAddress(server);

	AuthServerUserInfo info;

	g_NetworkManager->GetAuthServer()->GetUserInfo(info);

	packet->WriteString(info.nickname);

	SendPacket(packet);
}

INetworkSocket* CNetworkClient::GetSocket()
{
	return socket;
}

void CNetworkClient::SendPacket(INetworkPacket* packet)
{
	GetSocket()->SendPacket(packet);

	g_NetworkManager->GetPacketManager()->PushPacketToPull(packet);
}

void CNetworkClient::Update()
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

void CNetworkClient::PushPacketToMain(CNetworkPacket* packet)
{
	if (!from_thread_to_main.try_push(packet))
		delete packet;
}

CNetworkPacket* CNetworkClient::PopPacketFromThread()
{
	CNetworkPacket** ptr_packet = from_thread_to_main.front();

	if (ptr_packet == NULL)
		return NULL;

	CNetworkPacket* packet = *ptr_packet;

	from_thread_to_main.pop();

	return packet;
}

void CNetworkClient::HandlePacket(CNetworkPacket* packet)
{

}

void CNetworkClient::InitializeThread()
{
	client_thread = std::thread(ClientThread, this);
}

void CNetworkClient::ClientThread(CNetworkClient* client)
{
	NetworkIPAddress sender;

	auto socket = client->GetSocket();
	auto packet_manager = g_NetworkManager->GetPacketManager();

	while (client->client_thread_enabled.load())
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

			g_Log->LogError("ClientThread Receive failed: (%d) %s", error, g_NetworkManager->GetLastErrorString(error).c_str());
			break;
		}

		if (result == TS_OK)
			client->PushPacketToMain((CNetworkPacket*)packet);
	}
}
