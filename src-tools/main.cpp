#include <stdio.h>

#include "engine.hpp"

#include "game.hpp"

#include "modules/rendermodule.hpp"

IEngine* g_Engine = NULL;

class TrainSimTools : public IGame
{
public:
	TrainSimTools();
	~TrainSimTools() override;

    virtual void Start();
    virtual void RegisterComponents();
    virtual void PostStart();

    virtual void ProcessInput(); // before EarlyUpdate()
    virtual void EarlyUpdate(); // before update all entities
    virtual void Update(); // after update all entities and before late update all entities
    virtual void LateUpdate(); // after late update all entities
    virtual void Shutdown();
};

TrainSimTools::TrainSimTools()
{
}

TrainSimTools::~TrainSimTools()
{
}

void TrainSimTools::Start()
{
}
void TrainSimTools::RegisterComponents()
{
}
void TrainSimTools::PostStart()
{
    IRenderModule* render = g_Engine->GetModule<IRenderModule>();

    render->GetWindow()->SetTitle("TrainSimTools");
    render->GetWindow()->SetSize(glm::vec2(1280, 768));
}

void TrainSimTools::ProcessInput()
{
}
void TrainSimTools::EarlyUpdate()
{
}
void TrainSimTools::Update()
{
}
void TrainSimTools::LateUpdate()
{
}
void TrainSimTools::Shutdown()
{
}

int main()
{
	g_Engine = CreateEngine();

    g_Engine->SetGame(new TrainSimTools());
    g_Engine->Initialize();
    g_Engine->RunLoop();

	FreeEngine();

	return 0;
}