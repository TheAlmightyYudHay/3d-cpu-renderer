#pragma once

#include "Vector.h"
#include <stdint.h>
#include <stdbool.h>

extern "C" {
	#include "texture.h"
}

struct face_t {
	int a;
	int b;
	int c;

	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;

	Vector3 a_normal;
	Vector3 b_normal;
	Vector3 c_normal;

	Vector3 normal;
	uint32_t color;

	face_t() {
		a = 0;
		b = 0;
		c = 0;

		a_uv = { 0, 0 };
		b_uv = { 0, 0 };
		c_uv = { 0, 0 };

		a_normal = { 0, 0, 0 };
		b_normal = { 0, 0, 0 };
		c_normal = { 0, 0, 0 };

		normal = { 0, 0, 0 };
		color = 0xFFFF00FF;
	}
};

struct triangle_t  {
	Vector4 points[3];
	tex2_t texcoords[3];
	Vector3 normals[3];
	uint32_t color;

	triangle_t()
	{
		points[0] = { 0, 0, 0, 0 };
		points[1] = { 0, 0, 0, 0 };
		points[2] = { 0, 0, 0, 0 };

		texcoords[0] = { 0, 0 };
		texcoords[1] = { 0, 0 };
		texcoords[2] = { 0, 0 };

		normals[0] = { 0, 0, 0 };
		normals[1] = { 0, 0, 0 };
		normals[2] = { 0, 0, 0 };

		color = 0xFFFF00FF;
	}
};

Vector3 barycentric_weights(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& p);
void fill_triangle(triangle_t triangle, uint32_t color);
void draw_textured_triangle(triangle_t triangle, texture_t* texture, bool is_lit);