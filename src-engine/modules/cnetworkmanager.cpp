#include "cnetworkmanager.hpp"

#include "clogmodule.hpp"

#include "WinSock2.h"
#include "ws2tcpip.h"

#include <locale>
#include <codecvt>

#include "network/services/cauthserver.hpp"
#include "network/client/cnetworkclient.hpp"
#include "network/server/cnetworkserver.hpp"

CNetworkManager* g_NetworkManager = NULL;

CNetworkManager::CNetworkManager() : initialized(false), request_handler_enable(true), state(NETWORK_STATE_UNKNOWN), client(NULL), server(NULL), auth_server(NULL), requests_to_thread(256), requests_from_thread(256)
{
	g_NetworkManager = this;

	WSADATA ws_data;
	
	int status = WSAStartup(MAKEWORD(2, 2), &ws_data);

	if (status != 0)
	{
		g_Log->LogError("Failed to initialize WinSock!");
		g_Log->LogError("Error string [%d]: %s", status, GetLastErrorString(status).c_str());
		//return;
	}

	g_Log->LogInfo("WinSock initialized!");

	request_handler = std::thread(HTTPRequestHandler);

	auth_server = new CAuthServer("http://127.0.0.1:1337/");

	packet_manager = new CNetworkPacketManager();

	initialized = true;
}

CNetworkManager::~CNetworkManager()
{
	switch (state)
	{
	case NETWORK_STATE_IS_CLIENT:
		delete client;
		break;
	case NETWORK_STATE_IS_SERVER:
		delete server;
		break;
	case NETWORK_STATE_IS_SELF_HOSTED:
		delete client;
		delete server;
		break;
	case NETWORK_STATE_UNKNOWN:
	default:
		break;
	}

	delete auth_server;
	delete packet_manager;

	request_handler_enable.store(false);
	request_handler.join();
}

TSResult CNetworkManager::InitializeClient()
{
	if (state != NETWORK_STATE_UNKNOWN)
		return TS_ALREADY_USED;

	client = new CNetworkClient();

	state = NETWORK_STATE_IS_CLIENT;

	return TS_OK;
}

TSResult CNetworkManager::InitializeServer()
{
	if (state != NETWORK_STATE_UNKNOWN)
		return TS_ALREADY_USED;

	server = new CNetworkServer();

	state = NETWORK_STATE_IS_SERVER;

	return TS_OK;
}

TSResult CNetworkManager::InitializeSelfHost()
{
	if (state != NETWORK_STATE_UNKNOWN)
		return TS_ALREADY_USED;

	server = new CNetworkServer();
	client = new CNetworkClient();

	state = NETWORK_STATE_IS_SELF_HOSTED;

	return TS_OK;
}

TSResult CNetworkManager::Shutdown()
{
	if (client != NULL)
		delete client;

	if (server != NULL)
		delete server;

	state = NETWORK_STATE_UNKNOWN;

	return TS_OK;
}

INetworkServer* CNetworkManager::GetNetworkServer()
{
	return server;
}

INetworkClient* CNetworkManager::GetNetworkClient()
{
	return client;
}

IHTTPRequest* CNetworkManager::CreateHTTPRequest()
{
	return new CHTTPRequest();
}

INetworkPacket* CNetworkManager::CreatePacket(NetworkPacketID id)
{
	CNetworkPacket* packet = (CNetworkPacket*)packet_manager->PopPacketFromPull();

	packet->InitializeWritePacket();

	packet->WriteUInt8(id);

	return packet;
}

INetworkPacketManager* CNetworkManager::GetPacketManager()
{
	return packet_manager;
}

IAuthServer* CNetworkManager::GetAuthServer()
{
	return auth_server;
}

bool CNetworkManager::GetIsReady()
{
	return initialized;
}

int CNetworkManager::GetLastError()
{
	return WSAGetLastError();
}

std::string CNetworkManager::GetLastErrorString()
{
	int error = GetLastError();

	return GetLastErrorString(error);
}

std::string ConvertToUTF8(wchar_t* wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	return converter.to_bytes(std::wstring(wstr));
}

std::string CNetworkManager::GetLastErrorString(int error)
{
	wchar_t error_msg[1024];

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_msg, sizeof(error_msg), NULL);

	return ConvertToUTF8(error_msg);
}

std::string CNetworkManager::GetIPByHostname(const std::string& hostname)
{
	addrinfo hints, *res;

	int status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(hostname.c_str(), NULL, &hints, &res)) != 0)
	{
		g_Log->LogError("Failed getaddrinfo: %s", ConvertToUTF8(gai_strerror(status)).c_str());
		g_Log->LogError("getaddrinfo from \"%s\"", hostname.c_str());
		return "";
	}

	sockaddr_in* ip = (sockaddr_in*)res->ai_addr;
	char ip_address[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(ip->sin_addr), ip_address, INET_ADDRSTRLEN);

	freeaddrinfo(res);

	return std::string(ip_address);
}

NetworkIPAddress CNetworkManager::GetNetworkAddressByHostname(const std::string& hostname)
{
	auto ip = GetIPByHostname(hostname);

	if (ip == "")
		return NetworkIPAddress(255, 255, 255, 255);

	NetworkIPAddress address;

	sscanf(ip.c_str(), "%d.%d.%d.%d", 
		&address.address.octets.first,
		&address.address.octets.second,
		&address.address.octets.third,
		&address.address.octets.fourth);

	return address;
}

void CNetworkManager::Update()
{
	while (true)
	{
		if (requests_from_thread.empty()) break;

		auto request = *requests_from_thread.front();

		request->CallCallback();

		requests_from_thread.pop();
	}

	if (state == NETWORK_STATE_IS_SERVER || state == NETWORK_STATE_IS_SELF_HOSTED)
		server->Update();

	if (state == NETWORK_STATE_IS_CLIENT || state == NETWORK_STATE_IS_SELF_HOSTED)
		client->Update();
}

void CNetworkManager::AddHTTPRequestToQueue(CHTTPRequest* request)
{
	requests_to_thread.push(request);
}

void CNetworkManager::HTTPRequestHandler()
{
	auto that = g_NetworkManager;

	auto sleep_duration = std::chrono::milliseconds(10);

	while (that->request_handler_enable.load())
	{
		while (true)
		{
			if (that->requests_to_thread.empty()) break;

			auto request = *that->requests_to_thread.front();

			request->PerformAsync();

			that->requests_to_thread.pop();
			that->requests_from_thread.push(request);
		}

		std::this_thread::sleep_for(sleep_duration);
	}
}
