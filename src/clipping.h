#pragma once

#include "vector.h"
#include "texture.h"

#define MAX_NUM_POLY_VERTICES 10

enum {
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE
};

typedef struct {
	vec3_t point;
	vec3_t normal;
} plane_t;

typedef struct {
	vec3_t vertices[MAX_NUM_POLY_VERTICES];
	tex2_t texcoords[MAX_NUM_POLY_VERTICES];
	vec3_t normals[MAX_NUM_POLY_VERTICES];
	int num_vertices;
} polygon_t;

#define NUM_PLANES 6
extern plane_t frustum_planes[NUM_PLANES];

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t uv0, tex2_t uv1, tex2_t uv2, vec3_t n0, vec3_t n1, vec3_t n2);
void init_frustum_planes(float fovy, float fovx, float z_near, float z_far);
void clip_polygon_against_plain(polygon_t* polygon, int plane);
void clip_polygon(polygon_t* polygon);