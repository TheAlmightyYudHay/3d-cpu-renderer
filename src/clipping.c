#include "clipping.h"
#include <math.h>

extern plane_t frustum_planes[NUM_PLANES] = { 0 };

polygon_t create_polygon_from_triangle(
	vec3_t v0, vec3_t v1, vec3_t v2,
	tex2_t uv0, tex2_t uv1, tex2_t uv2,
	vec3_t n0, vec3_t n1, vec3_t n2
){
	return (polygon_t) {
		.vertices = {v0, v1, v2},
		.texcoords = {uv0, uv1, uv2},
		.normals = {n0, n1, n2},
		.num_vertices = 3
	};
}

void init_frustum_planes(float fovy, float fovx, float z_near, float z_far)
{
	float cos_half_fovy = cos(fovy / 2.0);
	float sin_half_fovy = sin(fovy / 2.0);
	float cos_half_fovx = cos(fovx / 2.0);
	float sin_half_fovx = sin(fovx / 2.0);

	frustum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_new(0,0,0);
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fovx;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fovx;
	
	frustum_planes[LEFT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fovx;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fovx;

	frustum_planes[TOP_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fovy;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fovy;

	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fovy;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fovy;

	frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
	frustum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

void clip_polygon_against_plain(polygon_t* polygon, int plane)
{
	vec3_t plane_point = frustum_planes[plane].point;
	vec3_t plane_normal = frustum_planes[plane].normal;

	vec3_t inside_vertices[MAX_NUM_POLY_VERTICES] = { 0 };
	vec3_t inside_normals[MAX_NUM_POLY_VERTICES] = { 0 };
	tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES] = { 0 };
	int num_inside_vertices = 0;

	vec3_t* current_vertex = &(polygon->vertices[0]);
	vec3_t* current_normal = &(polygon->normals[0]);
	tex2_t* current_texcoord = &(polygon->texcoords[0]);

	vec3_t* previous_vertex = &(polygon->vertices[polygon->num_vertices - 1]);
	vec3_t* previous_normal = &(polygon->normals[polygon->num_vertices - 1]);
	tex2_t* previous_texcoord = &(polygon->texcoords[polygon->num_vertices - 1]);

	float current_dot = 0.0;
	float previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

	while (current_vertex != &polygon->vertices[polygon->num_vertices])
	{
		current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

		if (current_dot * previous_dot < 0)
		{
			float t = previous_dot / (previous_dot - current_dot);
		
			vec3_t intersect_point = vec3_add(*previous_vertex, vec3_mul(vec3_sub(*current_vertex, *previous_vertex), t));
			vec3_t intersect_normal = vec3_add(*previous_normal, vec3_mul(vec3_sub(*current_normal, *previous_normal), t));

			vec3_normalize(&intersect_normal);

			vec2_t intersect_textcoord = vec2_add(
				(vec2_t) {previous_texcoord->u, previous_texcoord->v},
				vec2_mul(
					vec2_sub(
						(vec2_t) {current_texcoord->u, current_texcoord->v},
						(vec2_t) {previous_texcoord->u, previous_texcoord->v}
					), 
					t
				)
			);

			inside_vertices[num_inside_vertices] = intersect_point;
			inside_normals[num_inside_vertices] = intersect_normal;
			inside_texcoords[num_inside_vertices] = (tex2_t){.u = intersect_textcoord.x, .v = intersect_textcoord.y};
			num_inside_vertices += 1;
		}

		if (current_dot >= 0)
		{
			inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
			inside_normals[num_inside_vertices] = vec3_clone(current_normal);
			inside_texcoords[num_inside_vertices] = *current_texcoord;
			num_inside_vertices += 1;
		}

		previous_dot = current_dot;
		previous_vertex = current_vertex;
		previous_normal = current_normal;
		previous_texcoord = current_texcoord;
		current_vertex += 1;
		current_normal += 1;
		current_texcoord += 1;
	}

	polygon->num_vertices = num_inside_vertices;

	for (int i = 0; i < num_inside_vertices; i += 1)
	{
		polygon->vertices[i] = inside_vertices[i];
		polygon->normals[i] = inside_normals[i];
		polygon->texcoords[i] = inside_texcoords[i];
	}
}

void clip_polygon(polygon_t* polygon)
{
	clip_polygon_against_plain(polygon, RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plain(polygon, LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plain(polygon, TOP_FRUSTUM_PLANE);
	clip_polygon_against_plain(polygon, BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plain(polygon, NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plain(polygon, FAR_FRUSTUM_PLANE);
}
