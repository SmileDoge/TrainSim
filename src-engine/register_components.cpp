#include "register_components.hpp"

#include "modules/ccomponentfactory.hpp"

#include "entities/components/ctransform.hpp"
#include "entities/components/crendercomponent.hpp"

#define DEFINE_INFO() IComponentInfo info;

#define REGISTER_COMPONENT(component) \
info.name = #component; \
info.createComponent = CreateComponent_##component; \
info.deleteComponent = DeleteComponent_##component; \
g_ComponentFactory->RegisterComponent(info); \


DEFINE_CCOMPONENT_FUNCTIONS(Transform);
DEFINE_CCOMPONENT_FUNCTIONS(RenderComponent);

void RegisterComponents() // Register Standard Entity Components
{
	DEFINE_INFO();

	REGISTER_COMPONENT(Transform);
	REGISTER_COMPONENT(RenderComponent);
}