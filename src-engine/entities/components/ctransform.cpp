#include "ctransform.hpp"

CTransform::CTransform() : position(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f), matrix(1.0f)
{

}

CTransform::~CTransform()
{

}


void CTransform::SetPosition(glm::vec3& pos)
{
    position = pos;
}

void CTransform::SetRotation(glm::quat& rot)
{
    rotation = rot;
}

void CTransform::SetSize(glm::vec3& size)
{
    scale = size;
}

glm::vec3& CTransform::GetPosition()
{
    return position;
}

glm::quat& CTransform::GetRotation()
{
    return rotation;
}

glm::vec3& CTransform::GetSize()
{
    return scale;
}

void CTransform::SetLocalPosition(glm::vec3& pos)
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return SetPosition(pos);

    position = glm::vec3(glm::inverse(parent->GetMatrix()) * glm::vec4(pos, 1.0f));
}

void CTransform::SetLocalRotation(glm::quat& rot)
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return SetRotation(rot);

    glm::quat localRotation = glm::inverse(parent->GetRotation()) * rotation;

    rotation = localRotation * parent->GetRotation();
}

void CTransform::SetLocalSize(glm::vec3& size)
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return SetSize(size);

    scale = size / parent->GetSize();
}

glm::vec3 CTransform::GetLocalPosition()
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return position;

    return parent->GetMatrix() * glm::vec4(position, 1.0f);
}

glm::quat CTransform::GetLocalRotation()
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return rotation;

    return glm::inverse(parent->GetRotation()) * rotation;
}

glm::vec3 CTransform::GetLocalSize()
{
    ITransform* parent = GetParentTransform();

    if (parent == NULL)
        return scale;

    return scale * parent->GetSize();
}


void CTransform::SetMatrix(glm::mat4& mat)
{
    matrix = mat;

    UpdateTransform();
}

glm::mat4& CTransform::GetMatrix()
{
    UpdateMatrix();

    return matrix;
}

glm::mat4 CTransform::GetLocalMatrix()
{
    UpdateMatrix();

    if (GetParentTransform() == NULL)
        return matrix;

    return glm::inverse(GetParentTransform()->GetMatrix()) * matrix;
}

ITransform* CTransform::GetParentTransform()
{
    IEntity* parent = GetEntity()->GetParent();

    if (parent == NULL) return NULL;
    
    return parent->GetTransform();
}

void CTransform::UpdateTransform()
{
    position = glm::vec3(matrix[3]);

    rotation = glm::quat_cast(matrix);

    scale.x = glm::length(matrix[0]);
    scale.y = glm::length(matrix[1]);
    scale.z = glm::length(matrix[2]);
}

void CTransform::UpdateMatrix()
{
    matrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
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