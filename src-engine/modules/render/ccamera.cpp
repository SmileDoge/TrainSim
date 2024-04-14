#include "ccamera.hpp"

CCamera::CCamera(): rotation(glm::vec3(0.0f, 0.0f, 0.0f)), camera_location(0, 0, 0, 0, 0), view_mat(1.0f), proj_mat(1.0f), frustum_left(0.0f), frustum_right(0.0f), frustum_right_projected(0.0f)
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

	UpdateFrustum();
}

void CCamera::UpdateProjMatrix()
{
	if (type == CAMERA_TYPE_ORTHO)
		proj_mat = glm::ortho(left_ortho, right_ortho, bottom_ortho, top_ortho);
	else
		proj_mat = glm::perspective(glm::radians(fov), aspect, near, far);

	sky_proj_mat = glm::perspective(glm::radians(fov), aspect, 1000.0f, 6200.0f);

	frustum_right_projected.x = cosf(glm::radians(fov) / 2 * aspect);
	frustum_right_projected.z = sinf(glm::radians(fov) / 2 * aspect);

	UpdateFrustum();
}

void CCamera::UpdateFrustum()
{
	frustum_left.x = -view_mat[0][0] * frustum_right_projected.x + view_mat[0][2] * frustum_right_projected.z;
	frustum_left.y = -view_mat[1][0] * frustum_right_projected.x + view_mat[1][2] * frustum_right_projected.z;
	frustum_left.z = -view_mat[2][0] * frustum_right_projected.x + view_mat[2][2] * frustum_right_projected.z;

	frustum_left = glm::normalize(frustum_left);

	frustum_right.x = view_mat[0][0] * frustum_right_projected.x + view_mat[0][2] * frustum_right_projected.z;
	frustum_right.y = view_mat[1][0] * frustum_right_projected.x + view_mat[1][2] * frustum_right_projected.z;
	frustum_right.z = view_mat[2][0] * frustum_right_projected.x + view_mat[2][2] * frustum_right_projected.z;

	frustum_right = glm::normalize(frustum_right);
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

bool CCamera::InFOV(glm::vec3 center, float radius)
{
	center.x -= camera_location.Location.x;
	center.y -= camera_location.Location.y;
	center.z -= camera_location.Location.z;

	radius *= 2;

	if ((frustum_left.x * center.x + frustum_left.y * center.y - frustum_left.z * center.z) > radius)
		return false;

	if ((frustum_right.x * center.x + frustum_right.y * center.y - frustum_right.z * center.z) > radius)
		return false;

	return true;
	/*

	glm::vec4 rowX = view_proj_mat[0];
	glm::vec4 rowY = view_proj_mat[1];
	glm::vec4 rowZ = view_proj_mat[2];
	glm::vec4 rowW = view_proj_mat[3];

	glm::vec4 planes[6];

	planes[0] = rowW + rowX;
	planes[1] = rowW - rowX;
	planes[2] = rowW + rowY;
	planes[3] = rowW - rowY;
	planes[4] = rowW + rowZ;
	planes[5] = rowW - rowZ;

	for (int i = 0; i < 6; i++)
	{
		float distance = glm::dot(glm::vec3(planes[i]), center) + planes[i].w;

		if (distance < -radius)
			return false;
	}

	return true;
		*/
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
