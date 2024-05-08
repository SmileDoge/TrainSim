#pragma once

class ITrainCar;

class ITrainCabin
{
public:
	virtual ~ITrainCabin() = default;

	virtual ITrainCar* GetCar() = 0;
};