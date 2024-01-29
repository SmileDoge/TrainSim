#pragma once

#include "modules/render/camera.hpp"

#undef far
#undef near

class CCamera : public ICamera
{
public:
	CCamera();
	~CCamera() override;

	virtual void SetPosition(glm::vec3& pos);
	virtual glm::vec3& GetPosition();

	virtual void SetRotation(glm::quat& rotation);
	virtual glm::quat& GetRotation();

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

	glm::mat4 view_mat;
	glm::mat4 proj_mat;

	glm::vec3 position;
	glm::quat rotation;

	CameraType type;

	float fov, aspect, near, far;

	float left, right, bottom, top;
};