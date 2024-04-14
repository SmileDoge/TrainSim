#pragma once

class IModule
{
public:
	virtual ~IModule() = default;

	virtual void PostInit() {};
	virtual void Update() {};
	virtual void PreDeinit() {};

	int UpdateOrder = 0;
};