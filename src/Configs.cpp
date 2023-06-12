#include "Configs.h"

void Configs::Setup(float fov, float aspect_ratio, float near_plane, float far_plane)
{
	mRenderingMode.SetDefault();
	mProjectionMatrix = Matrix4x4::MakePerspective(fov, aspect_ratio, near_plane, far_plane);
}
