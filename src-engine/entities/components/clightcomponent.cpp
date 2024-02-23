#include "clightcomponent.hpp"

#include "modules/crendermodule.hpp"
#include "entities/components/transform.hpp"

CLightComponent::CLightComponent()
{
	light = new CLight();
}

CLightComponent::~CLightComponent()
{
	delete light;
}

void CLightComponent::Update()
{
    if (light == NULL) return;

    light->SetPosition(GetEntity()->GetTransform()->GetRelativeToCamera());

    g_Render->GetRenderFrame()->AddLight(light);
}

ILight* CLightComponent::GetLight()
{
    return light;
}