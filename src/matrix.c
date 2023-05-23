#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void)
{
	return (mat4_t) {
		.m = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		} 
	};
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
	return (mat4_t) {
		.m = {
			{sx,  0,  0, 0},
			{ 0, sy,  0, 0},
			{ 0,  0, sz, 0},
			{ 0,  0,  0, 1}
		}
	};
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
	return (mat4_t) {
		.m = {
			{1, 0, 0, tx},
			{0, 1, 0, ty},
			{0, 0, 1, tz},
			{0, 0, 0,  1}
		}
	};
}


mat4_t mat4_make_rotation_x(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return (mat4_t) {
		.m = {
			{1, 0,  0, 0},
			{0, c, -s, 0},
			{0, s,  c, 0},
			{0, 0,  0, 1}
		}
	};
}

mat4_t mat4_make_rotation_y(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return (mat4_t) {
		.m = {
			{ c, 0, s, 0},
			{ 0, 1, 0, 0},
			{-s, 0, c, 0},
			{ 0, 0, 0, 1}
		}
	};
}

mat4_t mat4_make_rotation_z(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return (mat4_t) {
		.m = {
			{c, -s, 0, 0},
			{s,  c, 0, 0},
			{0,  0, 1, 0},
			{0,  0, 0, 1}
		}
	};
}

mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2)
{
	mat4_t result = { 0 };

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			for (int i = 0; i < 4; i++)
			{
				result.m[row][col] += m1.m[row][i] * m2.m[i][col];
			}
		}
	}

	return result;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
	return (vec4_t) {
		.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
		.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
		.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
		.w = 1.0f,
	};
}

mat4_t mat4_make_perspective(float fov, float aspect, float zNear, float zFar)
{
	return (mat4_t) {
		.m = {
			{aspect/tan(fov/2.0),              0,                 0,                          0},
			{                  0, 1/tan(fov/2.0),                 0,                          0},
			{                  0,              0, zFar/(zFar-zNear), (-zFar*zNear)/(zFar-zNear)},
			{                  0,			   0,                 1.0,                          0}
		}
	};
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v)
{
	vec4_t projectedV = mat4_mul_vec4(mat_proj, v);

	if (projectedV.w != 0.0)
	{
		projectedV.x /= projectedV.w;
		projectedV.y /= projectedV.w;
		projectedV.z /= projectedV.w;
	}

	return projectedV;
}