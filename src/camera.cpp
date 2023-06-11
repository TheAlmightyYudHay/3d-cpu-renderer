#include "camera.h"

static camera_t camera = {
	/*.position =*/ {0, 0, 0},
	/*.direction =*/ {0, 0, 1},
	/*.yaw_angle =*/ 0.0,
	/*.pitch_angle =*/ 0.0
};

void change_camera_yaw(float yaw) { camera.yaw_angle += yaw; }
float get_camera_yaw() { return camera.yaw_angle; }

void change_camera_pitch(float pitch) { camera.pitch_angle += pitch; }
float get_camera_pitch() { return camera.pitch_angle; }

void change_camera_position(const Vector3& offset) { camera.position = camera.position + offset; }
Vector3 get_camera_position() { return camera.position; }

Vector3 get_camera_directon() { return camera.direction; }

void set_camera_direction(const Vector3& dir)
{
	camera.direction = dir;
}
