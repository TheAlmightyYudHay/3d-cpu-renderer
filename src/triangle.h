#pragma once

#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct {
	int a;
	int b;
	int c;

	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;

	vec3_t normal;
	uint32_t color;
} face_t;

typedef struct {
	vec4_t points[3];
	tex2_t texcoords[3];
	uint32_t color;
	float avg_depth;
} triangle_t;

void fill_triangle(triangle_t triangle, uint32_t color);

void draw_textured_triangle(triangle_t triangle, uint32_t* texture);