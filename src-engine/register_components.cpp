#include "register_components.hpp"

#include "modules/ccomponentfactory.hpp"
#include "modules/cresourcemanager.hpp"

#include "entities/components/ctransform.hpp"
#include "entities/components/crendercomponent.hpp"
#include "entities/components/clightcomponent.hpp"
#include "entities/components/cmodelrenderer.hpp"

#include "resources/cmodelresource.hpp"
#include "resources/ctextureresource.hpp"

DEFINE_CCOMPONENT_FUNCTIONS(Transform);
DEFINE_CCOMPONENT_FUNCTIONS(RenderComponent);
DEFINE_CCOMPONENT_FUNCTIONS(LightComponent);
DEFINE_CCOMPONENT_FUNCTIONS(ModelRenderer)

void RegisterComponents() // Register Standard Entity Components
{
	DEFINE_INFO();

	REGISTER_COMPONENT_ENGINE(Transform);
	REGISTER_COMPONENT_ENGINE(RenderComponent);
	REGISTER_COMPONENT_ENGINE(LightComponent);
	REGISTER_COMPONENT_ENGINE(ModelRenderer);

	g_ResourceManager->RegisterResourceFactory(new ModelResourceFactory());
	g_ResourceManager->RegisterResourceFactory(new TextureResourceFactory());
}