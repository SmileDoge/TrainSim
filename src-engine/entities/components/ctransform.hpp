#pragma once

#include "entities/components/transform.hpp"

class CTransform : public ITransform
{
public:
	CTransform();
	~CTransform() override;

    virtual int GetTileX();
    virtual int GetTileZ();
    virtual glm::vec3 GetLocation();
    virtual WorldPosition& GetWorldPosition();

    virtual void SetTileX(int tile);
    virtual void SetTileZ(int tile);
    virtual void SetLocation(glm::vec3& location);
    //virtual void Move(glm::vec3& direction);

    virtual glm::vec3 GetRelativeToCamera(ICamera* camera = NULL);

    virtual void SetRotation(glm::quat& rot);
    virtual glm::quat& GetRotation();

    virtual void SetSize(glm::vec3& size);
    virtual glm::vec3& GetSize();

    virtual glm::mat4 GetMatrix(ICamera* camera = NULL);

private:
    ITransform* GetParentTransform();
    void UpdateTransform();
    void UpdateMatrix();

    WorldPosition position;

    //glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    //glm::mat4 matrix;
};
