#pragma once

#include "entities/components/transform.hpp"

class CTransform : public ITransform
{
public:
	CTransform();
	~CTransform() override;

    virtual void SetPosition(glm::vec3& pos);
    virtual glm::vec3& GetPosition();

    virtual void SetRotation(glm::quat& rot);
    virtual glm::quat& GetRotation();

    virtual void SetSize(glm::vec3& size);
    virtual glm::vec3& GetSize();

    virtual void SetMatrix(glm::mat4& mat);
    virtual glm::mat4& GetMatrix();


    virtual void SetLocalPosition(glm::vec3& pos);
    virtual glm::vec3& GetLocalPosition();

    virtual void SetLocalRotation(glm::quat& rot);
    virtual glm::quat& GetLocalRotation();

    virtual void SetLocalSize(glm::vec3& size);
    virtual glm::vec3& GetLocalSize();

    virtual glm::mat4& GetLocalMatrix();

private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 size;
};
