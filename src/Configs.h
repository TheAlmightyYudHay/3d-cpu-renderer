#pragma once

#include "light.h"
#include "Vector.h"
#include "camera.h"
#include "Matrix.h"
#include "RenderingMode.h"

// Initial configs should be set here.
// Light
static const Vector3 LightDirection = { 0, -1, 0 };

// Camera
static const Vector3 CameraPosition = { 0, 0, 0 };
static const Vector3 CameraDirection = { 0, 0, 1 };

class Configs
{
public:
static Configs& GetInstance() {
		static Configs instance;
		return instance;
	}

	Light& GetLight() { return mLight; }
	Camera& GetCamera() { return mCamera; }
	Matrix4x4& GetProjectionMatrix() { return mProjectionMatrix; }
	RenderingMode& GetRenderingMode() { return mRenderingMode; }

	void Setup(float fov, float aspect_ratio, float near_plane, float far_plane);

private:
	Light mLight;
	Camera mCamera;
	Matrix4x4 mProjectionMatrix;
	RenderingMode mRenderingMode;

	// Should be private to avoid unwanted access.
	Configs() : mLight{ LightDirection, Vector3::Zero() }, mCamera{ CameraPosition, CameraDirection } {}

	Configs(const Configs&) = delete;
	Configs& operator=(const Configs&) = delete;
	Configs(const Configs&&) = delete;
};

