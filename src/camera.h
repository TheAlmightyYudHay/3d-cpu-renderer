#pragma once

#include "Vector.h"

class Camera
{
public:
	Camera(const Vector3& position, const Vector3& direction) : mPosition(position), mDirection(direction.Normalized()) {}

	Vector3 GetPosition() const { return mPosition; }
	Vector3 GetDirection() const { return mDirection; }
	void SetPosition(const Vector3& position) { mPosition = position; }
	void SetDirection(const Vector3& direction) { mDirection = direction; }

	void Move(const Vector3& offset) { mPosition = mPosition + offset; }

	float GetPitch() const { return mPitchAngle; }
	float GetYaw() const { return mYawAngle; }
	void ChangePitch(float pitch) { mPitchAngle += pitch; }
	void ChangeYaw(float yaw) { mYawAngle += yaw; }

private:
	Vector3 mPosition;
	Vector3 mDirection;
	float mYawAngle{ 0.0f };
	float mPitchAngle{ 0.0f };
};