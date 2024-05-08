#include "ctrainmanager.hpp"

#include "utils/wstring.hpp"

#include "modules/clogmodule.hpp"
#include "modules/cfilesystem.hpp"

#include "cengine.hpp"

#include "Windows.h"

CTrainManager::CTrainManager()
{
}

CTrainManager::~CTrainManager()
{
	for (auto& dll : dlls)
		UnloadControllerDLL(dll.first);
}

ITrain* CTrainManager::CreateTrain()
{
	return nullptr;
}

ITrainCar* CTrainManager::SpawnCar(ITrain* train, TrainDirection direction, TrainPlace place)
{
	return nullptr;
}

TSResult CTrainManager::LoadControllerDLL(const std::string& dll_path, TrainDLLInfo& result_info)
{
	HMODULE dll = LoadLibrary(TSUtils::ConvertToWSTRFromUTF8(dll_path).c_str());

	if (dll != NULL)
	{
		PROC_GetInfo get_info = (PROC_GetInfo)GetProcAddress(dll, "GetInfo");
	
		if (get_info == NULL)
		{
			FreeLibrary(dll);
			g_Log->LogError("LoadControllerDLL (%s) - Invalid GetInfo Function!", g_FileSystem->GetFileName(dll_path).c_str());
			return TS_INVALID_OPERATION;
		}

		TrainDLLInfo info = get_info();
	
		if (info.version.major != TRAIN_MANAGER_API.major || info.version.minor != TRAIN_MANAGER_API.minor)
		{
			FreeLibrary(dll);

			return TS_MISMATCH_VERSION;
		}

		PROC_CreateController create_controller = (PROC_CreateController)GetProcAddress(dll, "CreateController");

		if (create_controller == NULL)
		{
			FreeLibrary(dll);
			g_Log->LogError("LoadControllerDLL (%s) - Invalid CreateController Function!", g_FileSystem->GetFileName(dll_path).c_str());
			return TS_INVALID_OPERATION;
		}

		result_info = info;

		TrainDLL train_dll;

		train_dll.dll = dll;
		train_dll.dll_path = dll_path;
		train_dll.dll_name = info.dll_name;
		train_dll.proc_create = create_controller;
		train_dll.proc_get_info = get_info;
		train_dll.version = info.version;

		dlls[info.dll_name] = train_dll;

		return TS_OK;
	}

	return TS_NOT_FOUND;
}

TSResult CTrainManager::UnloadControllerDLL(const std::string& dll_name)
{
	if (dlls.find(dll_name) == dlls.end())
		return TS_NOT_FOUND;

	FreeLibrary((HMODULE)dlls[dll_name].dll);

	return TS_OK;
}

TSResult CTrainManager::GetControllerDLL(const std::string& dll_name, TrainDLL& train_dll)
{
	if (dlls.find(dll_name) == dlls.end())
		return TS_NOT_FOUND;

	train_dll = dlls[dll_name];

	return TS_OK;
}

TSResult CTrainManager::CreateController(const std::string& dll_name, ITrainController*& controller)
{
	if (dlls.find(dll_name) == dlls.end())
		return TS_NOT_FOUND;

	controller = dlls[dll_name].proc_create(g_Engine);

	return TS_OK;
}
