#pragma once

#include "traincar.hpp"

#include <vector>

enum TrainDirection : unsigned char
{
	TRAIN_DIRECTION_FORWARD,
	TRAIN_DIRECTION_BACKWARD,

	TRAIN_DIRECTION_DEFAULT = TRAIN_DIRECTION_FORWARD,
};

enum TrainPlace : unsigned char
{
	TRAIN_PLACE_FRONT,
	TRAIN_PLACE_BEHIND,

	TRAIN_PLACE_DEFAULT = TRAIN_PLACE_FRONT,
};

class ITrain
{
public:
	virtual ~ITrain() = default;

	virtual std::vector<ITrainCar*>& GetCars() = 0;
};