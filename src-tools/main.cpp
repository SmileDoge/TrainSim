#include <stdio.h>

#include "engine.hpp"

#include "game.hpp"

#include "modules/rendermodule.hpp"
#include "modules/logmodule.hpp"

#include "world.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"

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
    engine = g_Engine;

    ImGui::SetCurrentContext((ImGuiContext*)g_Engine->GetModule<IRenderModule>()->GetImGuiContext());
    glfwInit();
}
void TrainSimTools::RegisterComponents()
{
}
ILogModule* g_Log = NULL;
void TrainSimTools::PostStart()
{
    IRenderModule* render = g_Engine->GetModule<IRenderModule>();

    render->GetWindow()->SetTitle("TrainSimTools");
    render->GetWindow()->SetSize(glm::vec2(1366, 768));
    render->GetWindow()->SetType(WINDOW_TYPE_WINDOWED);
    render->GetWindow()->SetVSync(false);

    IWorld* world = g_Engine->GetWorld();

    world->SetSunDirection(glm::vec3(-1, 1, -2));

    g_Log = g_Engine->GetModule<ILogModule>();
}

#include "glm/glm.hpp"

glm::vec2 prevMousePos;
bool mousePressed;
bool mouseFirstTime;

float yaw, pitch;

void TrainSimTools::ProcessInput()
{
    float cameraSpeed = 6.0f * engine->GetDeltaTime();

    auto render = engine->GetModule<IRenderModule>();

    auto& cameraFront = render->GetCamera()->GetFront();
    auto& cameraUp = render->GetCamera()->GetUp();
    //auto& cameraPos = render->GetCamera()->GetPosition();

    auto direction = glm::vec3(0.0f);

    GLFWwindow* window = (GLFWwindow*)render->GetWindow()->GetGLFWPointer();

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 60.0f * engine->GetDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        cameraSpeed = 600.0f * engine->GetDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    render->GetCamera()->Move(direction);

    auto mousePress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    int mode = glfwGetInputMode(window, GLFW_CURSOR);

    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);

    glm::vec2 mousePos(xpos, ypos);

    if (mousePress == GLFW_PRESS && !mousePressed)
    {
        mousePressed = true;
        mouseFirstTime = true;
    }

    if (mousePress != GLFW_PRESS && mousePressed)
        mousePressed = false;

    if (mousePress == GLFW_PRESS && mousePressed)
    {
        if (mouseFirstTime)
            prevMousePos = mousePos;

        auto mouseDelta = mousePos - prevMousePos;

        float sensitivity = 0.1;

        mouseDelta *= sensitivity;

        yaw += mouseDelta.x;
        pitch += mouseDelta.y;

        if (pitch > 89.0f)
            pitch = 89.0f;

        if (pitch < -89.0f)
            pitch = -89.0f;

        if (yaw >= 360) yaw -= 360;
        if (yaw <= -360) yaw += 360;

        glm::quat rot = GLM_EULER(pitch, yaw, 0.0f);

        render->GetCamera()->SetRotation(rot);

        prevMousePos = mousePos;

        mouseFirstTime = false;
    }

    ImGui::Begin("Camera");

    auto& camera_pos = render->GetCamera()->GetLocation();
    auto camera_tile_x = render->GetCamera()->GetTileX();
    auto camera_tile_z = render->GetCamera()->GetTileZ();

    ImGui::Text("Tile: (%d, %d)", camera_tile_x, camera_tile_z);
    ImGui::Text("Position: (%f, %f, %f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("Rotation: (%f, %f, %f)", pitch, yaw, 0);

    static int tp_tile[2];

    ImGui::InputInt2("Teleport Tile", tp_tile);

    if (ImGui::Button("Teleport to tile"))
    {
        render->GetCamera()->SetTileX(tp_tile[0]);
        render->GetCamera()->SetTileZ(tp_tile[1]);
        g_Log->LogError("lel");
    }

    if (ImGui::Button("Teleport to (0,0,0)"))
    {
        render->GetCamera()->Move(-render->GetCamera()->GetLocation());
    }

    ImGui::End();

    ImGui::Begin("Graphics");
    ImGui::Text("Vertices drawn count: %d", render->GetTrianglesDrawnCount());
    ImGui::End();
}

int type;

void TrainSimTools::EarlyUpdate()
{
    if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
    {
        auto render = engine->GetModule<IRenderModule>();

        type++;

        if (type == 2) type = 0;

        g_Log->LogInfo("Current type %d", type);

        if (type == 0)
        {
            render->GetWindow()->SetSize(glm::vec2(1366, 768));
            render->GetWindow()->SetType(WINDOW_TYPE_WINDOWED);
        }

        if (type == 1)
        {
            render->GetWindow()->SetType(WINDOW_TYPE_WINDOWED_FULLSCREEN);
        }
    }
}

float fps = 1000;

#include "portable-file-dialogs.h"

#include "modules/resourcemanager.hpp"
#include "entities/components/modelrenderer.hpp"

#include "resources/modelresource.hpp"

IEntity* train = NULL;
TSModelResource* train_res = NULL;
std::string train_path;

void TrainSimTools::Update()
{
    ImGui::Begin(u8"Tools");
    
    ImGui::Text(u8"Текущая модель: %s", train_path.c_str());

    if (ImGui::Button(u8"Выбрать модель для просмотра"))
    {
        auto selection = pfd::open_file(u8"Выбрать модель").result();
    
        if (!selection.empty())
        {
            auto resourcefactory = engine->GetModule<IResourceManager>();
            auto world = engine->GetWorld();

            auto path = selection[0];

            auto model_load_options = ModelResourceLoadOptions();

            if (train == NULL)
            {
                train_path = path;

                train_res = resourcefactory->LoadResource<TSModelResource>(path, RESOURCE_LOAD_FLAG_DEFAULT, &model_load_options);

                train = world->CreateEntity();
                train->SetName("Train");
                train->CreateComponent<IModelRenderer>()->SetModel(train_res);
            }
            else
            {
                train_path = path;

                train_res = resourcefactory->LoadResource<TSModelResource>(path, RESOURCE_LOAD_FLAG_DEFAULT, &model_load_options);
            
                train->GetComponent<IModelRenderer>()->SetModel(train_res);
            }
        }
    }

    ImGui::End();

    IRenderModule* render = engine->GetModule<IRenderModule>();

    ImGui::Begin("Test", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("delta_time: %f ms, fps: %f", engine->GetDeltaTime() * 1000.f, 1 / engine->GetDeltaTime());
    ImGui::Text("Application average %f ms/frame (%f FPS)", engine->GetDeltaTime() * 1000.f, 1 / engine->GetDeltaTime());

    ImGui::SliderFloat("FPS Max", &fps, 10, 5000);

    engine->SetFPS(fps);

    glm::highp_vec3 sunColor = engine->GetWorld()->GetSunColor();
    glm::highp_vec3 ambientColor = engine->GetWorld()->GetAmbientColor();
    glm::highp_vec3 sunDir = engine->GetWorld()->GetSunDirection();

    ImGui::SliderFloat3("Sun Color", sunColor.data.data, 0, 1);
    ImGui::SliderFloat3("Ambient Color", ambientColor.data.data, 0, 1);
    ImGui::SliderFloat3("Sun Direction", sunDir.data.data, -1, 1);

    engine->GetWorld()->SetSunColor(sunColor);
    engine->GetWorld()->SetAmbientColor(ambientColor);
    engine->GetWorld()->SetSunDirection(sunDir);

    ImGui::End();
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