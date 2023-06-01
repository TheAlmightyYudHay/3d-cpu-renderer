#pragma once

#include "vector.h"

typedef struct {
	vec3_t position;
	vec3_t direction;
	float yaw_angle;
	float pitch_angle;
} camera_t;

void change_camera_yaw(float yaw);
float get_camera_yaw();
void change_camera_pitch(float pitch);
float get_camera_pitch();

void change_camera_position(vec3_t offset);
vec3_t get_camera_position();

vec3_t get_camera_directon();
void set_camera_direction(vec3_t dir);