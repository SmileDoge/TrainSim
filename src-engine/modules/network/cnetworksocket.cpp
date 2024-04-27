#include "cnetworksocket.hpp"

#include "modules/clogmodule.hpp"
#include "modules/cnetworkmanager.hpp"
#include "modules/network/cnetworkpacket.hpp"

CNetworkSocket::CNetworkSocket()
{
	InitializeSocket();

	receive_buffer = new unsigned char[PACKET_DATA_SIZE];
}

CNetworkSocket::~CNetworkSocket()
{
	closesocket(socket_internal);

	delete[] receive_buffer;
}

TSResult CNetworkSocket::Bind(unsigned short port)
{
	return BindWithIP("0.0.0.0", port);
}

TSResult CNetworkSocket::BindWithIP(const std::string& ip, unsigned short port)
{
	sockaddr_in bindaddr;

	bindaddr.sin_family = AF_INET;
	bindaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	bindaddr.sin_port = htons(port);

	int status = bind(socket_internal, (sockaddr*)&bindaddr, sizeof(bindaddr));

	if (status == SOCKET_ERROR)
	{
		g_Log->LogError("Bind error binding socket: %s", g_NetworkManager->GetLastErrorString().c_str());
		return TS_INVALID_OPERATION;
	}

	g_Log->LogDebug("Listening in port %d", port);

	return TS_OK;
}

size_t CNetworkSocket::SendBytes(unsigned char* bytes, size_t bytes_len, NetworkIPAddress& receiver)
{
	sockaddr_in sock_receiver;

	GetSockAddr(receiver, sock_receiver);

	int bytes_sent = sendto(socket_internal, (char*)bytes, bytes_len, 0, (sockaddr*)&sock_receiver, sizeof(sock_receiver));

	return bytes_sent;
}

int sockaddr_in_size = sizeof(sockaddr_in);

size_t CNetworkSocket::ReceiveBytes(unsigned char* buffer, size_t buffer_len, NetworkIPAddress& sender)
{
	sockaddr_in sock_sender;

	int bytes_received = recvfrom(socket_internal, (char*)buffer, buffer_len, 0, (sockaddr*)&sock_sender, &sockaddr_in_size);

	GetNetworkAddr(sock_sender, sender);

	return bytes_received;
}

TSResult CNetworkSocket::SendPacket(INetworkPacket* packet)
{
	SendBytes(packet->GetData(), packet->Size(), packet->GetAddress());

	return TS_OK;
}

TSResult CNetworkSocket::ReceivePacket(INetworkPacket* packet)
{
	CNetworkPacket* c_packet = (CNetworkPacket*)packet;

	NetworkIPAddress sender;

	size_t received = ReceiveBytes(receive_buffer, PACKET_DATA_SIZE, sender);

	if (received == -1) return TS_INVALID_OPERATION;

	c_packet->InitializeReadPacket(receive_buffer, received);

	c_packet->SetAddress(sender);

	return TS_OK;
}

inline void CNetworkSocket::GetSockAddr(NetworkIPAddress& in, sockaddr_in& out)
{
	out.sin_family = AF_INET;
	out.sin_addr.S_un.S_addr = in.address.full;
	out.sin_port = htons(in.port);
}

inline void CNetworkSocket::GetNetworkAddr(sockaddr_in& in, NetworkIPAddress& out)
{
	out.address.full = in.sin_addr.S_un.S_addr;
	out.port = ntohs(in.sin_port);
}

void CNetworkSocket::InitializeSocket()
{
	socket_internal = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (socket_internal == SOCKET_ERROR)
	{
		g_Log->LogError("InitializeSocket error creating socket: %s", g_NetworkManager->GetLastErrorString().c_str());
		return;
	}
}
