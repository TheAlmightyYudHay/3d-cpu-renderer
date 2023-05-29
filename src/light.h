#pragma once

#include "vector.h"
#include <stdint.h>

typedef struct {
	vec3_t direction;
	vec3_t view;
} light_t;

extern light_t light;

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);