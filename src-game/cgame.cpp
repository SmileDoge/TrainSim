#include "cgame.hpp"
#include "modules/logmodule.hpp"
#include "engine.hpp"

#include "world.hpp"
#include "entities/component.hpp"

#include "modules/componentfactory.hpp"
#include "modules/rendermodule.hpp"
#include "modules/filesystem.hpp"
#include "modules/componentfactory.hpp"
#include "modules/resourcemanager.hpp"

#include "modules/render/renderframe.hpp"

#include "entities/components/rendercomponent.hpp"
#include "entities/components/transform.hpp"
#include "entities/components/lightcomponent.hpp"
#include "entities/components/modelrenderer.hpp"

#include "resources/modelresource.hpp"

#include "imgui.h"

#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

DEFINE_COMPONENT_FUNCTIONS(TestComponent)

ILogModule* g_Log = NULL;
IEngine* g_Engine = NULL;

TrainSimGame::TrainSimGame(IEngine* eng)
{
	engine = eng;

    g_Engine = eng;

    fps = 1000;
}

TrainSimGame::~TrainSimGame()
{

}

void TrainSimGame::Start()
{
	g_Log = engine->GetModule<ILogModule>();

    ImGui::SetCurrentContext((ImGuiContext*)engine->GetModule<IRenderModule>()->GetImGuiContext());
    glfwInit();
}

IEntity* entity = NULL;
IEntity* train_2es4k = NULL;
ILightComponent* light = NULL;
ITexture* texture = NULL;

void TrainSimGame::PostStart()
{
    IWorld* world = engine->GetWorld();

    IEntity* light_entity = world->CreateEntity();

    light = light_entity->CreateComponent<ILightComponent>();

    IRenderModule* render = engine->GetModule<IRenderModule>();

    render->GetWindow()->SetSize(glm::vec2(1600, 900));
    render->GetWindow()->SetType(WINDOW_TYPE_WINDOWED);

    /*
    float vertices[] = {
        // координаты        // нормали           // текстурные координаты
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    IFileSystem* filesystem = engine->GetModule<IFileSystem>();

    auto vertexShader = filesystem->ReadFileString("data/shaders/testshader.vs");
    auto fragmentShader = filesystem->ReadFileString("data/shaders/testshader.fs");

    IShader* shader = render->CreateShader("exampleshader");
    shader->SetVertexShader(vertexShader.c_str());
    shader->SetFragmentShader(fragmentShader.c_str());
    shader->Compile();

    IMesh* mesh = render->GetMeshManager()->CreateMesh();
    mesh->SetData(vertices, 36, NULL, 0);

    mesh->SetName("examplemesh");

    IMaterial* material = render->GetMaterialManager()->CreateMaterial("examplematerial");
    material->SetShader(shader);

    entity = world->CreateEntity();

    entity->SetName("Test Entity");
    */

    ICamera* camera = render->GetCamera();

    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->SetRotation(GLM_EULER(0.0f, 0.0f, 0.0f));

    auto window = render->GetWindow();

    window->SetTitle("TrainSim C++/OpenGL");

    window->LoadIcon("data/images/kek.jpg");


    auto resourcefactory = engine->GetModule<IResourceManager>();

    auto start_time = engine->GetSysTime();

    auto es4k = resourcefactory->LoadResource<TSModelResource>("data/trains/mdd_2ES4k-084/mdd_2es4ka.ts_model", RESOURCE_LOAD_FLAG_DEFAULT);
    auto chs4 = resourcefactory->LoadResource<TSModelResource>("data/trains/zdsLoco_chs4z-080/zdsLoco_chs4z-080.ts_model", RESOURCE_LOAD_FLAG_DEFAULT);

    auto end_time = engine->GetSysTime();

    g_Log->LogWarn("Resources Loaded in %lf sec", (end_time - start_time));

    
    train_2es4k = world->CreateEntity();
    train_2es4k->SetName("2ES4K");
    train_2es4k->CreateComponent<IModelRenderer>()->SetModel(es4k);
    
    IEntity* train_chs4 = world->CreateEntity();
    train_chs4->SetName("CHS4");
    train_chs4->CreateComponent<IModelRenderer>()->SetModel(chs4);
    train_chs4->GetTransform()->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));
    

    /*
    for (int i = 0; i < 100; i++)
    {
        IEntity* extrain = world->CreateEntity();

        extrain->SetName("2ES4K");

        extrain->CreateComponent<IModelRenderer>()->SetModel(res);
    }

    for (int i = 0; i < 2000; i++)
    {
        char name[100];

        snprintf(name, 100, "test-texture-%d", i);

        auto test_tex = render->GetTextureManager()->CreateTexture(name);

        test_tex->SetFilter(TEXTURE_FILTER_LINEAR, TEXTURE_FILTER_LINEAR);
        test_tex->SetWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
        test_tex->SetData(width, height, channels == 4 ? TEXTURE_FORMAT_RGBA : TEXTURE_FORMAT_RGB, data);

        //render->GetTextureManager()->DeleteTexture(test_tex);
        //test_tex->GenerateMipmap();
    }

    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    USHORT indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    IFileSystem* filesystem = engine->GetModule<IFileSystem>();

    filesystem->WriteFileString("data/shaders/test.txt", "test\n123\tkek\nanother line");

    auto vertexShader = filesystem->ReadFileString("data/shaders/testshader.vs");
    auto fragmentShader = filesystem->ReadFileString("data/shaders/testshader.fs");

    IShader* shader = render->CreateShader("exampleshader");
    shader->SetVertexShader(vertexShader.c_str());
    shader->SetFragmentShader(fragmentShader.c_str());
    shader->Compile();

    IMesh* mesh = render->GetMeshManager()->CreateMesh();
    mesh->SetData(vertices, 4, indices, 6);

    mesh->SetName("examplemesh");

    IMaterial* material = render->GetMaterialManager()->CreateMaterial("examplematerial");
    material->SetShader(shader);

    entity = world->CreateEntity();

    entity->SetName("Test Entity");

    IRenderComponent* rendercomponent = entity->CreateComponent<IRenderComponent>();
    rendercomponent->SetMesh(mesh);
    rendercomponent->SetMaterial(material);

    auto transform = entity->GetTransform();

    transform->SetRotation(GLM_EULER(0.0f, 0.0f, 0.0f));
    //glm::mat4 model = glm::mat4(1.0f);
    //transform->SetMatrix(glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    ICamera* camera = render->GetCamera();

    camera->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));

    int width, height, channels;

    size_t file_len;
    uint8_t* file_data = filesystem->ReadFile("data/images/kek.jpg", &file_len);

    uint8_t* data = stbi_load_from_memory(file_data, file_len, &width, &height, &channels, 0);

    delete file_data;

    texture = render->GetTextureManager()->CreateTexture("exampletexture");

    texture->SetFilter(TEXTURE_FILTER_LINEAR, TEXTURE_FILTER_LINEAR);
    texture->SetWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
    texture->SetData(width, height, channels == 4 ? TEXTURE_FORMAT_RGBA : TEXTURE_FORMAT_RGB, data);
    texture->GenerateMipmap();

    stbi_image_free(data);

    material->SetTexture(MATERIAL_ALBEDO_TEXTURE, texture);
    */
    /*
    for (int i = 0; i < 2000; i++)
    {
        IEntity* ent = world->CreateEntity();

        ent->SetName("Render Entity");

        IRenderComponent* rendercomponent = ent->CreateComponent<IRenderComponent>();

        rendercomponent->SetMesh(mesh);
        rendercomponent->SetMaterial(material);

        ITransform* trans = ent->GetTransform();

        TestComponent* testcomponent = ent->CreateComponent<TestComponent>();

        testcomponent->SetIndex(i);
    }
    */
}

void TrainSimGame::RegisterComponents()
{
    IComponentFactory* factory = engine->GetModule<IComponentFactory>();

    DEFINE_COMPONENT_INFO();

    //REGISTER_COMPONENT(factory, TestComponent);
}

#include "glm/glm.hpp"

glm::vec2 prevMousePos;
bool mousePressed;
bool mouseFirstTime;

float yaw, pitch;

void TrainSimGame::ProcessInput()
{
    float cameraSpeed = 2.5f * engine->GetDeltaTime();

    auto render = engine->GetModule<IRenderModule>();

    auto& cameraFront = render->GetCamera()->GetFront();
    auto& cameraUp = render->GetCamera()->GetUp();
    auto& cameraPos = render->GetCamera()->GetPosition();

    GLFWwindow* window = (GLFWwindow*)render->GetWindow()->GetGLFWPointer();

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 6.0f * engine->GetDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    render->GetCamera()->SetPosition(cameraPos);

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

        yaw -= mouseDelta.x;
        pitch += mouseDelta.y;

        if (pitch > 89.0f)
            pitch = 89.0f;

        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::quat rot = GLM_EULER(pitch, yaw, 0.0f);

        render->GetCamera()->SetRotation(rot);

        prevMousePos = mousePos;

        mouseFirstTime = false;
    }

    ImGui::Begin("Camera");

    auto& camera_pos = render->GetCamera()->GetPosition();

    ImGui::Text("Position: (%f, %f, %f)", camera_pos.x, camera_pos.y, camera_pos.z);
    ImGui::Text("Rotation: (%f, %f, %f)", pitch, yaw, 0);

    ImGui::End();
}

int type;

void TrainSimGame::EarlyUpdate()
{
    if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
    {
        auto render = engine->GetModule<IRenderModule>();

        type++;

        if (type == 3) type = 0;

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

        if (type == 2)
        {
            render->GetWindow()->SetType(WINDOW_TYPE_FULLSCREEN);
        }
    }
}

void TrainSimGame::Update()
{
    ImGui::ShowDemoWindow();

    //ImGui::SetNextWindowSize(ImVec2(400, 720), ImGuiCond_Always);
    ImGui::Begin("Test", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("delta_time: %f ms, fps: %f", engine->GetDeltaTime() * 1000.f, 1 / engine->GetDeltaTime());
    ImGui::Text("Application average %f ms/frame (%f FPS)", engine->GetDeltaTime() * 1000.f, 1 / engine->GetDeltaTime());

    ImGui::SliderFloat("FPS Max", &fps, 1, 5000);
    
    engine->SetFPS(fps);

    ImGui::End();

    //ImGui::Begin("cinnamoroll");
    //ImGui::Image((void*)texture->GetID(), ImVec2(800, 800));
    //ImGui::End();
    
    if (train_2es4k != NULL)
    {
        ImGui::Begin("Train");

        auto& pos = train_2es4k->GetTransform()->GetPosition();
        auto& ang = glm::eulerAngles(train_2es4k->GetTransform()->GetRotation());
        auto& mat = train_2es4k->GetTransform()->GetMatrix();
        //auto mat = glm::mat4(1.0f);

        ImGui::Text("Position: (%f, %f, %f)", pos.x, pos.y, pos.z);
        ImGui::Text("Angle: (%f, %f, %f)", glm::degrees(ang.x), ang.y, ang.z);
        ImGui::Spacing();
        ImGui::Text("Matrix:\n[%.1f; %.1f; %.1f; %.1f]\n[%.1f; %.1f; %.1f; %.1f]\n[%.1f; %.1f; %.1f; %.1f]\n[%.1f; %.1f; %.1f; %.1f]",
            mat[0][0], mat[0][1], mat[0][2], mat[0][3],
            mat[1][0], mat[1][1], mat[1][2], mat[1][3],
            mat[2][0], mat[2][1], mat[2][2], mat[2][3],
            mat[3][0], mat[3][1], mat[3][2], mat[3][3]
        );

        float pos_x = pos.x;
        float pos_y = pos.y;
        float pos_z = pos.z;

        ImGui::SliderFloat("X", &pos_x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &pos_y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &pos_z, -10.0f, 10.0f);

        train_2es4k->GetTransform()->SetPosition(glm::vec3(pos_x, pos_y, pos_z));

        ImGui::End();
    }

    ImGui::Begin("Light");

    auto light_pos = light->GetEntity()->GetTransform()->GetPosition();

    ImGui::Text("Position: (%f, %f, %f)", light_pos.x, light_pos.y, light_pos.z);

    float light_pos_x = light_pos.x;
    float light_pos_y = light_pos.y;
    float light_pos_z = light_pos.z;

    ImGui::SliderFloat("X", &light_pos_x, -10.0f, 10.0f);
    ImGui::SliderFloat("Y", &light_pos_y, -10.0f, 10.0f);
    ImGui::SliderFloat("Z", &light_pos_z, -10.0f, 10.0f);

    light->GetEntity()->GetTransform()->SetPosition(glm::vec3(light_pos_x, light_pos_y, light_pos_z));

    ImGui::End();

    /*
    ImGui::Begin("RenderFrame");

    auto render = engine->GetModule<IRenderModule>();

    auto items = render->GetRenderFrame()->GetItems();

    for (auto& [material, list] : items)
    {
        ImGui::Text("Material: %s (%p)", material->GetName().c_str(), material);
        
        for (auto& item : list)
        {
            ImGui::Text("    Item: mesh_name: %s (mesh_ptr: %p)", item.Mesh->GetName().c_str(), item.Mesh);
        }
    }

    ImGui::End();
    */

    //g_Log->LogInfo("delta_time: %f, fps: %f", engine->GetDeltaTime(), 1/engine->GetDeltaTime());
}

void TrainSimGame::LateUpdate()
{

}

void TrainSimGame::Shutdown()
{

}

void TestComponent::Update()
{
    auto transform = GetEntity()->GetTransform();

    float x = cos(g_Engine->GetCurTime() + index);
    float y = sin(g_Engine->GetCurTime() + index);

    transform->SetPosition(glm::vec3(x, y, 0));
}

void TestComponent::SetIndex(int index)
{
    this->index = index;
}