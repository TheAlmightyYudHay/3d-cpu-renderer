#include "vector.h"
#include <math.h>

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};

	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) + v.z * sin(angle),
		.y = v.y,
		.z = -v.x * sin(angle) + v.z * cos(angle)
	};

	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};

	return rotated_vector;
}

///////////////////////////////////////////////////////////////////////////////////////
// Vector 2D functions implementations
///////////////////////////////////////////////////////////////////////////////////////

float vec2_length_squared(vec2_t v)
{
	return v.x * v.x + v.y * v.y;
}


float vec2_length(vec2_t v)
{
	return sqrtf(vec2_length_squared(v));
}

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
	return (vec2_t){
		.x = v1.x + v2.x, 
		.y = v1.y + v2.y
	};
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2)
{
	return (vec2_t){
		.x = v1.x - v2.x, 
		.y = v1.y - v2.y
	};
}

vec2_t vec2_mul(vec2_t v, float factor)
{
	return (vec2_t){
		.x = v.x * factor,
		.y = v.y * factor
	};
}

vec2_t vec2_div(vec2_t v, float factor)
{
	return vec2_mul(v, 1.0f / factor);
}

double vec2_dot(vec2_t v1, vec2_t v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

void vec2_normalize(vec2_t* v)
{
	float factor = 1.0 / vec2_length(*v);
	v->x *= factor;
	v->y *= factor;
}

///////////////////////////////////////////////////////////////////////////////////////
// Vector 3D functions implementations
///////////////////////////////////////////////////////////////////////////////////////

float vec3_length_squared(vec3_t v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}


float vec3_length(vec3_t v)
{
	return sqrtf(vec3_length_squared(v));
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
	return (vec3_t){
		.x = v1.x + v2.x, 
		.y = v1.y + v2.y, 
		.z = v1.z + v2.z
	};
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
	return (vec3_t){
		.x = v1.x - v2.x, 
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
}

vec3_t vec3_mul(vec3_t v, float factor)
{
	return (vec3_t){
		.x = v.x * factor,
		.y = v.y * factor,
		.z = v.z * factor
	};
}

vec3_t vec3_div(vec3_t v, float factor)
{
	return vec3_mul(v, 1.0f / factor);
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
	return (vec3_t) {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	};
}

double vec3_dot(vec3_t v1, vec3_t v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void vec3_normalize(vec3_t* v)
{
	float factor = 1.0 / vec3_length(*v);
	v->x *= factor;
	v->y *= factor;
	v->z *= factor;
}


///////////////////////////////////////////////////////////////////////////////////////
// Vector conversion functions implementations
///////////////////////////////////////////////////////////////////////////////////////
vec4_t vec4_from_vec3(vec3_t v)
{
	return (vec4_t) {.x = v.x, .y = v.y, .z = v.z, .w = 1.0f};
}

vec3_t vec3_from_vec4(vec4_t v)
{
	return (vec3_t) { .x = v.x, .y = v.y, .z = v.z };
}

