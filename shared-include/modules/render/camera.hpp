#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

enum CameraType
{
	CAMERA_TYPE_ORTHO,
	CAMERA_TYPE_PERSPECTIVE
};

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual void SetPosition(glm::vec3& pos) = 0;
	virtual glm::vec3& GetPosition() = 0;

	virtual void SetRotation(glm::quat& rotation) = 0;
	virtual glm::quat& GetRotation() = 0;

	virtual void SetFront(glm::vec3& front) = 0;
	virtual glm::vec3& GetFront() = 0;

	virtual void SetUp(glm::vec3& up) = 0;
	virtual glm::vec3& GetUp() = 0;

	virtual glm::vec3& GetRight() = 0;

	virtual void SetType(CameraType type) = 0;
	virtual CameraType GetType() = 0;

	virtual void SetFOV(float fov) = 0;
	virtual float GetFOV() = 0;

	virtual void SetAspect(float aspect) = 0;
	virtual float GetAspect() = 0;

	virtual void SetSize(float left, float right, float bottom, float top) = 0;
	virtual void GetSize(float& left, float& right, float& bottom, float& top) = 0;

	virtual glm::mat4& GetViewMatrix() = 0;
	virtual glm::mat4& GetProjectionMatrix() = 0;

	virtual void Update() {};
};