#include "crendercomponent.hpp"
#include "modules/crendermodule.hpp"
#include "entities/components/transform.hpp"

CRenderComponent::CRenderComponent() : mesh(NULL), material(NULL)
{

}

CRenderComponent::~CRenderComponent()
{

}

void CRenderComponent::Update()
{
    if (mesh == NULL) return;
    if (material == NULL) return;

    g_Render->GetRenderFrame()->AddRenderItem(mesh, material, GetEntity()->GetTransform()->GetMatrix());
}

void CRenderComponent::SetMesh(IMesh* mesh)
{
    this->mesh = mesh;
}

IMesh* CRenderComponent::GetMesh()
{
    return mesh;
}

void CRenderComponent::SetMaterial(IMaterial* material)
{
    this->material = material;
}

IMaterial* CRenderComponent::GetMaterial()
{
    return material;
}

/*

    virtual void SetMesh(IMesh* mesh);
    virtual IMesh* GetMesh();

    virtual void SetMaterial(IMaterial* material);
    virtual IMaterial* GetMaterial();
    */