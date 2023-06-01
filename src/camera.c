#include "camera.h"

static camera_t camera = {
	.position = {0, 0, 0},
	.direction = {0, 0, 1},
	.yaw_angle = 0.0,
	.pitch_angle = 0.0
};

void change_camera_yaw(float yaw) { camera.yaw_angle += yaw; }
float get_camera_yaw() { return camera.yaw_angle; }

void change_camera_pitch(float pitch) { camera.pitch_angle += pitch; }
float get_camera_pitch() { return camera.pitch_angle; }

void change_camera_position(vec3_t offset) { camera.position = vec3_add(camera.position, offset); }
vec3_t get_camera_position() { return camera.position; }

vec3_t get_camera_directon() { return camera.direction; }

void set_camera_direction(vec3_t dir)
{
	camera.direction = dir;
}
