#pragma once

#include "global.hpp"

#include "engine.hpp"

#include "module.hpp"

#include "train.hpp"
#include "traincar.hpp"

#include <string>

#define TRAIN_MANAGER_API TSVersion(0, 1, 0)

struct TrainDLLInfo
{
	std::string dll_name;

	TSVersion target_api;
	TSVersion version;
};

typedef ITrainController* (*PROC_CreateController)(IEngine* engine);
typedef TrainDLLInfo (*PROC_GetInfo)(void);

struct TrainDLL
{
	std::string dll_name;
	std::string dll_path;

	TSVersion version;

	PROC_CreateController proc_create;
	PROC_GetInfo proc_get_info;

	void* dll;
};

#define REGISTER_DLL_CONTROLLER(type, name, ver) \
ENGINE_API ENGINE_EXPORT ITrainController* CreateController(IEngine* engine) \
{ \
	g_Engine = engine; \
	return new ExampleTrainController(); \
} \
ENGINE_API ENGINE_EXPORT TrainDLLInfo GetInfo() \
{ \
	TrainDLLInfo info; \
	info.dll_name = name; \
	info.version = ver; \
	info.target_api = TRAIN_MANAGER_API; \
	return info; \
}

class ITrainManager : public IModule
{
public:
	virtual ~ITrainManager() = default;

	virtual ITrain* CreateTrain() = 0;
	virtual ITrainCar* SpawnCar(ITrain* train, TrainDirection direction = TRAIN_DIRECTION_DEFAULT, TrainPlace place = TRAIN_PLACE_DEFAULT) = 0;

	virtual TSResult LoadControllerDLL(const std::string& dll_path, TrainDLLInfo& result_info) = 0;
	virtual TSResult UnloadControllerDLL(const std::string& dll_name) = 0;

	virtual TSResult GetControllerDLL(const std::string& dll_name, TrainDLL& train_dll) = 0;

	virtual TSResult CreateController(const std::string& dll_name, ITrainController*& controller) = 0;
};