#pragma once

#include "traincabin.hpp"

class ITrainCar;

class ITrainController
{
public:
	virtual ~ITrainController() = default;

	ITrainCar* GetCar() { return car; };
	ITrainCabin* GetCabin() { return cabin; };

protected:
	ITrainCar* car;
	ITrainCabin* cabin;
};