#pragma once

#include "vector.h"
#include <stdint.h>
#include "texture.h"
#include <stdbool.h>

typedef struct {
	int a;
	int b;
	int c;

	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;

	vec3_t a_normal;
	vec3_t b_normal;
	vec3_t c_normal;

	vec3_t normal;
	uint32_t color;
} face_t;

typedef struct {
	vec4_t points[3];
	tex2_t texcoords[3];
	vec3_t normals[3];
	uint32_t color;
} triangle_t;

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);
void fill_triangle(triangle_t triangle, uint32_t color);
void draw_textured_triangle(triangle_t triangle, uint32_t* texture, bool is_lit);