#pragma once

#include "traincabin.hpp"
#include "traincontroller.hpp"

#include "entities/entity.hpp"

class ITrain;

class ITrainCar
{
public:
	virtual ~ITrainCar() = default;

	virtual ITrain* GetTrain() = 0;
	virtual ITrainCabin* GetCabin() = 0;
	virtual ITrainController* GetController() = 0;

	virtual ITrainCar* GetForwardCar() = 0;
	virtual ITrainCar* GetBackwardCar() = 0;

	virtual IEntity* GetEntity() = 0;
};