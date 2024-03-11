#include "ccamera.hpp"

CCamera::CCamera(): rotation(glm::vec3(0.0f, 0.0f, 0.0f)), camera_location(0, 0, 0, 0, 0), view_mat(1.0f), proj_mat(1.0f)
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
}

CCamera::~CCamera()
{

}

int CCamera::GetTileX()
{
	return camera_location.TileX;
}

int CCamera::GetTileZ()
{
	return camera_location.TileZ;
}

glm::vec3 CCamera::GetLocation()
{
	return camera_location.Location;
}

WorldLocation& CCamera::GetWorldLocation()
{
	return camera_location;
}

void CCamera::SetTileX(int tile)
{
	camera_location.TileX = tile;
}

void CCamera::SetTileZ(int tile)
{
	camera_location.TileZ = tile;
}

void CCamera::Move(glm::vec3& direction)
{
	camera_location.Location += direction;
	camera_location.Normalize();

	UpdateViewMatrix();
}

void CCamera::UpdateViewMatrix()
{
	glm::mat4 rot = glm::toMat4(rotation);

	up = glm::vec3(rot[1]);
	front = glm::vec3(rot[2]);

	auto& location = camera_location.Location;

	view_mat = glm::lookAt(location, location + front, up);
}

void CCamera::UpdateProjMatrix()
{
	if (type == CAMERA_TYPE_ORTHO)
		proj_mat = glm::ortho(left_ortho, right_ortho, bottom_ortho, top_ortho);
	else
		proj_mat = glm::perspective(glm::radians(fov), aspect, near, far);

	sky_proj_mat = glm::perspective(glm::radians(fov), aspect, 1000.0f, 6200.0f);
}


void CCamera::SetType(CameraType type)
{
	this->type = type;

	UpdateProjMatrix();
}

/*
void CCamera::SetPosition(glm::vec3& pos)
{
	position = pos;

	UpdateViewMatrix();
}
*/

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

/*
glm::vec3& CCamera::GetPosition()
{
	return position;
}
*/

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

glm::mat4& CCamera::GetSkyProjectionMatrix()
{
	return sky_proj_mat;
}
