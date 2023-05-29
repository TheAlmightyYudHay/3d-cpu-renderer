#include "camera.h"

extern camera_t camera = {
	.position = {0, 0, 0},
	.direction = {0, 0, 1},
	.forward_velocity = {0, 0, 0},
	.yaw_angle = 0.0
};