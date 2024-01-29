#include "ccamera.hpp"

CCamera::CCamera(): rotation(glm::vec3(0.0f, 0.0f, 0.0f)), position(0.0f, 0.0f, 0.0f), view_mat(1.0f), proj_mat(1.0f)
{
	left = 0;
	right = 0;
	bottom = 0;
	top = 0;

	type = CAMERA_TYPE_PERSPECTIVE;

	fov = 70;
}

CCamera::~CCamera()
{

}

void CCamera::UpdateViewMatrix()
{
	glm::mat4 rot = glm::toMat4(rotation);

	auto front = glm::vec3(rot * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	auto up = glm::vec3(rot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

	view_mat = glm::lookAt(position, position + front, up);
}

void CCamera::UpdateProjMatrix()
{
	if (type == CAMERA_TYPE_ORTHO)
		proj_mat = glm::ortho(left, right, bottom, top);
	else
		proj_mat = glm::perspective(glm::radians(fov), aspect, near, far);
}


void CCamera::SetType(CameraType type)
{
	this->type = type;

	UpdateProjMatrix();
}

void CCamera::SetPosition(glm::vec3& pos)
{
	position = pos;

	UpdateViewMatrix();
}

void CCamera::SetRotation(glm::quat& rot)
{
	rotation = rot;

	UpdateViewMatrix();
}

void CCamera::SetFOV(float fov)
{
	this->fov = fov;

	UpdateProjMatrix();
}

void CCamera::SetAspect(float aspect)
{
	this->aspect = aspect;

	UpdateProjMatrix();
}

void CCamera::SetNearFar(float near, float far)
{
	this->near = near;
	this->far = far;

	UpdateProjMatrix();
}

void CCamera::SetSize(float left, float right, float bottom, float top)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;

	UpdateProjMatrix();
}


CameraType CCamera::GetType()
{
	return type;
}

glm::vec3& CCamera::GetPosition()
{
	return position;
}

glm::quat& CCamera::GetRotation()
{
	return rotation;
}

float CCamera::GetFOV()
{
	return fov;
}

float CCamera::GetAspect()
{
	return aspect;
}

void CCamera::GetNearFar(float& near, float& far)
{
	near = this->near;
	far = this->far;
}

void CCamera::GetSize(float& left, float& right, float& bottom, float& top)
{
	left = this->left;
	right = this->right;
	bottom = this->bottom;
	top = this->top;
}


glm::mat4& CCamera::GetViewMatrix()
{
	return view_mat;
}

glm::mat4& CCamera::GetProjectionMatrix()
{
	return proj_mat;
}

/*
virtual void SetTransform(glm::mat4& mat);

virtual void SetPosition(glm::vec3& pos);
virtual glm::vec3& GetPosition();

virtual void SetRotation(glm::quat& rotation);
virtual glm::quat& GetRotation();

virtual void SetType(CameraType type);
virtual CameraType GetType();

virtual void SetFOV(float fov);
virtual float GetFOV();

virtual void SetSize(float left, float right, float bottom, float top);
virtual void GetSize(float& left, float& right, float& bottom, float& top);
*/