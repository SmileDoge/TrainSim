#pragma once

class IModule
{
public:
	virtual ~IModule() = default;

	virtual void PostInit() = 0;
	virtual void Update() = 0;
	virtual void PreDeinit() = 0;

	int UpdateOrder = 0;
};