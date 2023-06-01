#pragma once

#include "vector.h"
#include <stdint.h>

typedef struct {
	vec3_t direction;
	vec3_t view;
} light_t;

vec3_t get_light_direction(void);
vec3_t get_light_view(void);
void set_view_light(vec3_t value);
void normalize_light_directon(void);

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);