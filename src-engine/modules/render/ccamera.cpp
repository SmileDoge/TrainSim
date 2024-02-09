#include "ccamera.hpp"

CCamera::CCamera(): rotation(glm::vec3(0.0f, 0.0f, 0.0f)), position(0.0f, 0.0f, 0.0f), view_mat(1.0f), proj_mat(1.0f)
{
	left_ortho = 0;
	right_ortho = 0;
	bottom_ortho = 0;
	top_ortho = 0;

	type = CAMERA_TYPE_PERSPECTIVE;

	fov = 70;

	near = 0.1f;
	far = 100.f;

	UpdateViewMatrix();
	UpdateProjMatrix();
	//proj_mat = glm::perspective(glm::radians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);
}

CCamera::~CCamera()
{

}

void CCamera::UpdateViewMatrix()
{
	glm::mat4 rot = glm::toMat4(rotation);

	//auto front = glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	up = glm::vec3(rot[1]);
	front = glm::vec3(rot[2]);
	right = glm::vec3(rot[0]);

	//view_mat = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
	view_mat = glm::lookAt(position, position + front, up);
	//view_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, -3.0f));
}

void CCamera::UpdateProjMatrix()
{
	if (type == CAMERA_TYPE_ORTHO)
		proj_mat = glm::ortho(left_ortho, right_ortho, bottom_ortho, top_ortho);
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
	this->left_ortho = left;
	this->right_ortho = right;
	this->bottom_ortho = bottom;
	this->top_ortho = top;

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

void CCamera::UpdateTransform()
{
	rotation = glm::quatLookAt(front, up);

	rotation = glm::normalize(rotation);
}

void CCamera::SetFront(glm::vec3& front)
{
	this->front = front;

	UpdateTransform();

	UpdateViewMatrix();
}

glm::vec3& CCamera::GetFront()
{
	return front;
}

void CCamera::SetUp(glm::vec3& up)
{
	this->up = up;

	UpdateTransform();

	UpdateViewMatrix();
}

glm::vec3& CCamera::GetUp()
{
	return up;
}

glm::vec3& CCamera::GetRight()
{
	return right;
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
	left = this->left_ortho;
	right = this->right_ortho;
	bottom = this->bottom_ortho;
	top = this->top_ortho;
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