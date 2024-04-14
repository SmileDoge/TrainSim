#pragma once

#include "coordinates.hpp"

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

	virtual int GetTileX() = 0;
	virtual int GetTileZ() = 0;
	virtual glm::vec3 GetLocation() = 0;
	virtual WorldLocation& GetWorldLocation() = 0;

	virtual void SetTileX(int tile) = 0;
	virtual void SetTileZ(int tile) = 0;

	virtual void Move(glm::vec3& direction) = 0;

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

	virtual void SetNearFar(float near, float far) = 0;
	virtual void GetNearFar(float& near, float& far) = 0;

	virtual void SetSize(float left, float right, float bottom, float top) = 0;
	virtual void GetSize(float& left, float& right, float& bottom, float& top) = 0;

	virtual bool InFOV(glm::vec3 center, float radius) = 0;

	virtual glm::mat4& GetViewMatrix() = 0;
	virtual glm::mat4& GetProjectionMatrix() = 0;

	virtual glm::mat4& GetSkyProjectionMatrix() = 0;

	virtual void Update() {};
};