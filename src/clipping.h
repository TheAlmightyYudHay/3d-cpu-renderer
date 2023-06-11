#pragma once

#include "Vector.h"

extern "C" {
	#include "texture.h"
}

#define MAX_NUM_POLY_VERTICES 10

enum {
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE
};

struct plane_t {
	Vector3 point;
	Vector3 normal;
};

struct polygon_t {
	Vector3 vertices[MAX_NUM_POLY_VERTICES];
	tex2_t texcoords[MAX_NUM_POLY_VERTICES];
	Vector3 normals[MAX_NUM_POLY_VERTICES];
	int num_vertices;
};

#define NUM_PLANES 6
extern plane_t frustum_planes[NUM_PLANES];

polygon_t create_polygon_from_triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, tex2_t uv0, tex2_t uv1, tex2_t uv2, const Vector3& n0, const Vector3& n1, const Vector3& n2);
void init_frustum_planes(float fovy, float fovx, float z_near, float z_far);
void clip_polygon_against_plain(polygon_t* polygon, int plane);
void clip_polygon(polygon_t* polygon);