#include "trains/trainmanager.hpp"

#include "modules/logmodule.hpp"

IEngine* g_Engine = NULL;

class ExampleTrainController : public ITrainController
{
public:
	ExampleTrainController();
	~ExampleTrainController() override;
};

ExampleTrainController::ExampleTrainController()
{
	ILogModule* log = g_Engine->GetModule<ILogModule>();

	log->LogDebug("ExampleTrainController()");
}

ExampleTrainController::~ExampleTrainController()
{
	ILogModule* log = g_Engine->GetModule<ILogModule>();

	log->LogDebug("~ExampleTrainController()");
}

REGISTER_DLL_CONTROLLER(ExampleTrainController, "example-train", TSVersion(0, 1, 0));