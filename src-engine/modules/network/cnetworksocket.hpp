#pragma once

#include "modules/network/networksocket.hpp"

#include "WinSock2.h"

class CNetworkSocket : public INetworkSocket
{
public:
	CNetworkSocket();
	~CNetworkSocket() override;

	virtual TSResult Bind(unsigned short port);
	virtual TSResult BindWithIP(const std::string& ip, unsigned short port);

	virtual size_t SendBytes(unsigned char* bytes, size_t bytes_len, NetworkIPAddress& receiver);
	virtual size_t ReceiveBytes(unsigned char* buffer, size_t buffer_len, NetworkIPAddress& sender);

	virtual TSResult SendPacket(INetworkPacket* packet);
	virtual TSResult ReceivePacket(INetworkPacket* packet);
private:

	void GetSockAddr(NetworkIPAddress& in, sockaddr_in& out);
	void GetNetworkAddr(sockaddr_in& in, NetworkIPAddress& out);

	void InitializeSocket();

	SOCKET socket_internal;
	unsigned char* receive_buffer;
};