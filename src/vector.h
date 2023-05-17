#pragma once

typedef struct
{
	float x;
	float y;
} vec2_t;

typedef struct
{
	int x;
	int y;
} vec2_int_t;

typedef struct
{
	float x;
	float y;
	float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

///////////////////////////////////////////////////////////////////////////////////////
// Vector 2D functions
///////////////////////////////////////////////////////////////////////////////////////
float vec2_length(vec2_t v);
float vec2_length_squared(vec2_t v);
vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_sub(vec2_t v1, vec2_t v2);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
double vec2_dot(vec2_t v1, vec2_t v2);
void vec2_normalize(vec2_t* v);


///////////////////////////////////////////////////////////////////////////////////////
// Vector 3D functions
///////////////////////////////////////////////////////////////////////////////////////
float vec3_length(vec3_t v);
float vec3_length_squared(vec3_t v);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_sub(vec3_t v1, vec3_t v2);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
double vec3_dot(vec3_t v1, vec3_t v2);
void vec3_normalize(vec3_t* v);