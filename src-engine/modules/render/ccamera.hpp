#pragma once

#include "modules/render/camera.hpp"

#undef far
#undef near

class CCamera : public ICamera
{
public:
	CCamera();
	~CCamera() override;

	virtual int GetTileX();
	virtual int GetTileZ();
	virtual glm::vec3 GetLocation();
	virtual WorldLocation& GetWorldLocation();

	virtual void Move(glm::vec3& direction);

	virtual void SetRotation(glm::quat& rotation);
	virtual glm::quat& GetRotation();

	virtual void SetFront(glm::vec3& front);
	virtual glm::vec3& GetFront();

	virtual void SetUp(glm::vec3& up);
	virtual glm::vec3& GetUp();

	virtual glm::vec3& GetRight();

	virtual void SetType(CameraType type);
	virtual CameraType GetType();

	virtual void SetFOV(float fov);
	virtual float GetFOV();

	virtual void SetAspect(float aspect);
	virtual float GetAspect();

	virtual void SetNearFar(float near, float far);
	virtual void GetNearFar(float& near, float& far);

	virtual void SetSize(float left, float right, float bottom, float top);
	virtual void GetSize(float& left, float& right, float& bottom, float& top);

	virtual glm::mat4& GetViewMatrix();
	virtual glm::mat4& GetProjectionMatrix();

private:
	void UpdateViewMatrix();
	void UpdateProjMatrix();

	void UpdateTransform();

	glm::mat4 view_mat;
	glm::mat4 proj_mat;

	WorldLocation camera_location;
	//glm::vec3 position;

	glm::quat rotation;

	glm::vec3 front, right, up;

	CameraType type;

	float fov, aspect, near, far;

	float left_ortho, right_ortho, bottom_ortho, top_ortho;
};