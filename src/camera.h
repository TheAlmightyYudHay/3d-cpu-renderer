#pragma once

#include "Vector.h"

struct camera_t {
	Vector3 position;
	Vector3 direction;
	float yaw_angle;
	float pitch_angle;
};

void change_camera_yaw(float yaw);
float get_camera_yaw();
void change_camera_pitch(float pitch);
float get_camera_pitch();

void change_camera_position(const Vector3& offset);
Vector3 get_camera_position();

Vector3 get_camera_directon();
void set_camera_direction(const Vector3& dir);