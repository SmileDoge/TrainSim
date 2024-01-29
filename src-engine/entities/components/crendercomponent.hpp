#pragma once

#include "entities/components/rendercomponent.hpp"

#include "modules/render/cmesh.hpp"
#include "modules/render/cmaterial.hpp"

class CRenderComponent : public IRenderComponent
{
public:
	CRenderComponent();
	~CRenderComponent() override;

    virtual void Update();

    virtual void SetMesh(IMesh* mesh);
    virtual IMesh* GetMesh();

    virtual void SetMaterial(IMaterial* material);
    virtual IMaterial* GetMaterial();

private:
    IMesh* mesh;
    IMaterial* material;
};
