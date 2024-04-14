#pragma once

class INetworkSocket
{
public:
	virtual ~INetworkSocket() = default;

	virtual void Bind() = 0;
};