#pragma once

#include "Vector.h"
#include <stdint.h>

struct light_t {
	Vector3 direction;
	Vector3 view;
};

Vector3 get_light_direction(void);
Vector3 get_light_view(void);
void set_view_light(const Vector3& value);
void normalize_light_directon(void);

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);