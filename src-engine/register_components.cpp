#include "register_components.hpp"
#include "modules/ccomponentfactory.hpp"
#include "entities/components/examplecomponent.hpp"

#define REGISTER_COMPONENT(name) g_ComponentFactory->RegisterComponent(#name, CreateComponent_##name, DeleteComponent_##name);

void RegisterComponents() // Register Standard Entity Components
{
	//g_ComponentFactory->RegisterComponent("ExampleComponent", CreateComponent_ExampleComponent, DeleteComponent_ExampleComponent);
	// OR
	REGISTER_COMPONENT(ExampleComponent)
}