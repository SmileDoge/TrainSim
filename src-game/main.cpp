#include <iostream>
#include "engine.hpp"
#include "world.hpp"
#include "modules/rendermodule.hpp"
#include "modules/componentfactory.hpp"

void ShowEngineInfo(IEngine* engine)
{
    short major, minor, patch;

    char* date, * time, * compiler;

    long version = 0;

    engine->GetBuildInfo(date, time, version, compiler);
    engine->GetEngineVersion(major, minor, patch);

    std::cout << "Engine Date Build = " << date << std::endl;
    std::cout << "Engine Time Build = " << time << std::endl;
    std::cout << "Engine Version " << major << "." << minor << "." << patch << std::endl;
}

int main()
{
    IEngine* engine = CreateEngine();

    ShowEngineInfo(engine);

    IEntity* root = engine->GetWorld()->CreateEntity();
    IEntity* children_root_1 = engine->GetWorld()->CreateEntity();
    IEntity* children_root_2 = engine->GetWorld()->CreateEntity();
    IEntity* children_1_1 = engine->GetWorld()->CreateEntity();

    root->SetParent(NULL);
        children_root_1->SetParent(root);
            children_1_1->SetParent(children_root_1);
        children_root_2->SetParent(root);
    
    root->SetName("Root");
    children_root_1->SetName("Children of Root #1");
    children_root_2->SetName("Children of Root #2");
    children_1_1->SetName("Children of child #1 from root");

    printf("Children of Root:\n");
    for (auto& child1 : root->GetChildrens())
    {
        printf("\tEntity: %s\n\t Childrens:\n", child1->GetName().c_str());
        for (auto& child2 : child1->GetChildrens())
        {
            printf("\t\tEntity: %s\n", child2->GetName().c_str());
        }
    }

    engine->RunLoop();

    FreeEngine();
}
/*
int main()
{
    std::cout << "Initializing Engine..." << std::endl;

    IEngine* engine = CreateEngine();

    ShowEngineInfo(engine);

    
    IComponentFactory* factory = (IComponentFactory*)engine->GetModule("ComponentFactory");

    IComponent* example = factory->CreateComponent("ExampleComponent");

    example->Start();
    example->OnDestroy();

    IEntity* entity = example->GetEntity();

    std::cout << "Entity: " << entity << std::endl;

    factory->DeleteComponent(example);

    engine->RunLoop();

    FreeEngine();
}
*/