#include "clipping.h"
#include <math.h>

extern plane_t frustum_planes[NUM_PLANES] = {};

polygon_t create_polygon_from_triangle(
	const Vector3& v0, const Vector3& v1, const Vector3& v2,
	tex2_t uv0, tex2_t uv1, tex2_t uv2, 
	const Vector3& n0, const Vector3& n1, const Vector3& n2
){
	return polygon_t {
		/*.vertices =*/ {v0, v1, v2},
		/*.texcoords =*/ {uv0, uv1, uv2},
		/*.normals =*/ {n0, n1, n2},
		/*.num_vertices =*/ 3
	};
}

void init_frustum_planes(float fovy, float fovx, float z_near, float z_far)
{
	float cos_half_fovy = cos(fovy / 2.0);
	float sin_half_fovy = sin(fovy / 2.0);
	float cos_half_fovx = cos(fovx / 2.0);
	float sin_half_fovx = sin(fovx / 2.0);

	frustum_planes[RIGHT_FRUSTUM_PLANE].point = Vector3{};
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.SetX(-cos_half_fovx);
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.SetY(0);
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.SetZ(sin_half_fovx);
	
	frustum_planes[LEFT_FRUSTUM_PLANE].point = Vector3{};
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.SetX(cos_half_fovx);
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.SetY(0);
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.SetZ(sin_half_fovx);

	frustum_planes[TOP_FRUSTUM_PLANE].point = Vector3{};
	frustum_planes[TOP_FRUSTUM_PLANE].normal.SetX(0);
	frustum_planes[TOP_FRUSTUM_PLANE].normal.SetY(-cos_half_fovy);
	frustum_planes[TOP_FRUSTUM_PLANE].normal.SetZ(sin_half_fovy);

	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = Vector3{};
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.SetX(0);
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.SetY(cos_half_fovy);
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.SetZ(sin_half_fovy);

	frustum_planes[NEAR_FRUSTUM_PLANE].point = Vector3{ 0, 0, z_near };
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.SetX(0);
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.SetY(0);
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.SetZ(1);

	frustum_planes[FAR_FRUSTUM_PLANE].point = Vector3{ 0, 0, z_far };
	frustum_planes[FAR_FRUSTUM_PLANE].normal.SetX(0);
	frustum_planes[FAR_FRUSTUM_PLANE].normal.SetY(0);
	frustum_planes[FAR_FRUSTUM_PLANE].normal.SetZ(-1);
}

void clip_polygon_against_plain(polygon_t* polygon, int plane)
{
	Vector3 plane_point = frustum_planes[plane].point;
	Vector3 plane_normal = frustum_planes[plane].normal;

	Vector3 inside_vertices[MAX_NUM_POLY_VERTICES];
	Vector3 inside_normals[MAX_NUM_POLY_VERTICES];
	tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES];
	int num_inside_vertices = 0;

	Vector3* current_vertex = &(polygon->vertices[0]);
	Vector3* current_normal = &(polygon->normals[0]);
	tex2_t* current_texcoord = &(polygon->texcoords[0]);

	Vector3* previous_vertex = &(polygon->vertices[polygon->num_vertices - 1]);
	Vector3* previous_normal = &(polygon->normals[polygon->num_vertices - 1]);
	tex2_t* previous_texcoord = &(polygon->texcoords[polygon->num_vertices - 1]);

	float current_dot = 0.0;
	float previous_dot = Vector3::Dot(*previous_vertex-  plane_point, plane_normal);

	while (current_vertex != &polygon->vertices[polygon->num_vertices])
	{
		current_dot = Vector3::Dot(*current_vertex - plane_point, plane_normal);

		if (current_dot * previous_dot < 0)
		{
			float t = previous_dot / (previous_dot - current_dot);
		
			Vector3 intersect_point = *previous_vertex + t * (*current_vertex - *previous_vertex);
			Vector3 intersect_normal = *previous_normal + t * (*current_normal - *previous_normal);

			intersect_normal.Normalize();

			Vector2 intersect_textcoord =
				Vector2 {previous_texcoord->u, previous_texcoord->v}
				+ t * (
						Vector2{current_texcoord->u, current_texcoord->v} 
						- Vector2{previous_texcoord->u, previous_texcoord->v}
					  );

			inside_vertices[num_inside_vertices] = intersect_point;
			inside_normals[num_inside_vertices] = intersect_normal;
			inside_texcoords[num_inside_vertices] = tex2_t{intersect_textcoord.GetX(), intersect_textcoord.GetY()};
			num_inside_vertices += 1;
		}

		if (current_dot >= 0)
		{
			inside_vertices[num_inside_vertices] = *current_vertex;
			inside_normals[num_inside_vertices] = *current_normal;
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
