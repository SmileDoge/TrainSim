#include "cgame.hpp"
#include "modules/logmodule.hpp"
#include "engine.hpp"

#include "world.hpp"
#include "entities/component.hpp"
#include "modules/componentfactory.hpp"

#include "modules/rendermodule.hpp"

#include "entities/components/rendercomponent.hpp"

#include "imgui.h"

ILogModule* g_Log = NULL;

TrainSimGame::TrainSimGame(IEngine* eng)
{
	engine = eng;

    fps = 60;
}

TrainSimGame::~TrainSimGame()
{

}

void TrainSimGame::Start()
{
	g_Log = engine->GetModule<ILogModule>();

	g_Log->LogDebug("TrainSimGame::Start()");

    ImGui::SetCurrentContext((ImGuiContext*)engine->GetModule<IRenderModule>()->GetImGuiContext());
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.8f, 0.2f, 1.0f);\n"
"}\n\0";

void TrainSimGame::PostStart()
{
    IWorld* world = engine->GetWorld();

    IEntity* entity = world->CreateEntity();

    entity->SetName("Test Entity");

    IRenderModule* render = engine->GetModule<IRenderModule>();

    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    USHORT indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    IShader* shader = render->CreateShader("exampleshader");
    shader->SetVertexShader(vertexShaderSource);
    shader->SetFragmentShader(fragmentShaderSource);
    shader->Compile();

    IMesh* mesh = render->GetMeshManager()->CreateMesh();
    mesh->SetData(vertices, 4, indices, 6);

    mesh->SetName("examplemesh");

    IMaterial* material = render->GetMaterialManager()->CreateMaterial("examplematerial");
    material->SetShader(shader);

    IRenderComponent* rendercomponent = (IRenderComponent*)entity->CreateComponent("RenderComponent");
    rendercomponent->SetMesh(mesh);
    rendercomponent->SetMaterial(material);
}

void TrainSimGame::RegisterComponents()
{
	g_Log->LogDebug("TrainSimGame::RegisterComponents()");
}

void TrainSimGame::ProcessInput()
{

}

void TrainSimGame::EarlyUpdate()
{

}

void TrainSimGame::Update()
{
    ImGui::ShowDemoWindow();

    //ImGui::SetNextWindowSize(ImVec2(400, 720), ImGuiCond_Always);
    ImGui::Begin("Test", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("delta_time: %f, fps: %f", engine->GetDeltaTime(), 1 / engine->GetDeltaTime());
    ImGui::Text("Application average %f ms/frame (%f FPS)", engine->GetDeltaTime(), 1 / engine->GetDeltaTime());

    ImGui::SliderFloat("FPS Max", &fps, 1, 5000);
    
    engine->SetFPS(fps);

    ImGui::End();

    //g_Log->LogInfo("delta_time: %f, fps: %f", engine->GetDeltaTime(), 1/engine->GetDeltaTime());
}

void TrainSimGame::LateUpdate()
{

}

void TrainSimGame::Shutdown()
{
	g_Log->LogDebug("TrainSimGame::Shutdown()");
}
