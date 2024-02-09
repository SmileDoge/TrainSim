#pragma once

#include "entities/component.hpp"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

class ITransform: public IComponent
{
public:
    virtual ~ITransform() = default;

    virtual void SetPosition(glm::vec3& pos) = 0;
    virtual glm::vec3& GetPosition() = 0;

    virtual void SetRotation(glm::quat& rot) = 0;
    virtual glm::quat& GetRotation() = 0;

    virtual void SetSize(glm::vec3& size) = 0;
    virtual glm::vec3& GetSize() = 0;

    virtual void SetMatrix(glm::mat4& mat) = 0;
    virtual glm::mat4& GetMatrix() = 0;


    virtual void SetLocalPosition(glm::vec3& pos) = 0;
    virtual glm::vec3 GetLocalPosition() = 0;

    virtual void SetLocalRotation(glm::quat& rot) = 0;
    virtual glm::quat GetLocalRotation() = 0;

    virtual void SetLocalSize(glm::vec3& size) = 0;
    virtual glm::vec3 GetLocalSize() = 0;

    virtual glm::mat4 GetLocalMatrix() = 0;
};