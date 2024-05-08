#pragma once

#include "trains/trainmanager.hpp"

class CTrainManager : public ITrainManager
{
public:
	CTrainManager();
	~CTrainManager() override;

	virtual ITrain* CreateTrain();
	virtual ITrainCar* SpawnCar(ITrain* train, TrainDirection direction = TRAIN_DIRECTION_DEFAULT, TrainPlace place = TRAIN_PLACE_DEFAULT);

	virtual TSResult LoadControllerDLL(const std::string& dll_path, TrainDLLInfo& result_info);
	virtual TSResult UnloadControllerDLL(const std::string& dll_name);

	virtual TSResult GetControllerDLL(const std::string& dll_name, TrainDLL& train_dll);

	virtual TSResult CreateController(const std::string& dll_name, ITrainController*& controller);

private:
	std::map<std::string, TrainDLL> dlls;
};