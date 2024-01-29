#include "ctransform.hpp"

CTransform::CTransform()
{

}

CTransform::~CTransform()
{

}


void CTransform::SetPosition(glm::vec3& pos)
{

}

void CTransform::SetRotation(glm::quat& rot)
{

}

void CTransform::SetSize(glm::vec3& size)
{

}

glm::vec3& CTransform::GetPosition()
{
    return glm::vec3();
}

glm::quat& CTransform::GetRotation()
{
    return glm::quat();
}

glm::vec3& CTransform::GetSize()
{
    return glm::vec3();
}


void CTransform::SetLocalPosition(glm::vec3& pos)
{

}

void CTransform::SetLocalRotation(glm::quat& rot)
{

}

void CTransform::SetLocalSize(glm::vec3& size)
{

}

glm::vec3& CTransform::GetLocalPosition()
{
    return glm::vec3();
}

glm::quat& CTransform::GetLocalRotation()
{
    return glm::quat();
}

glm::vec3& CTransform::GetLocalSize()
{
    return glm::vec3();
}


void CTransform::SetMatrix(glm::mat4& mat)
{

}

glm::mat4& CTransform::GetMatrix()
{
    return glm::mat4(1.0f);
}

glm::mat4& CTransform::GetLocalMatrix()
{
    return glm::mat4(1.0f);
}
/*

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
*/