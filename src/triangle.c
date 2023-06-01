#include "triangle.h"
#include <stdio.h>
#include <math.h>
#include "display.h"
#include "swap.h"
#include "light.h"

int safe_pixel = 0;

float get_slope_inversed(vec2_int_t from, vec2_int_t to)
{
	if (to.y == from.y) return 0;
	return (to.x - from.x) / (float)(to.y - from.y);
}

void draw_triangle_pixel(
	int x0, int y0, float z0, float w0,
	int x1, int y1, float z1, float w1,
	int x2, int y2, float z2, float w2,
	int x, int y, uint32_t color
) {
	vec3_t weights = barycentric_weights(
		(vec2_t) {
		x0, y0
	},
		(vec2_t) {
		x1, y1
	},
		(vec2_t) {
		x2, y2
	},
		(vec2_t) {
		x, y
	}
	);

	float interpolated_reciprocal_w = 1.0 / w0 * weights.x + 1.0 / w1 * weights.y + 1.0 / w2 * weights.z;

	if (x >= get_window_width() || y >= get_window_height() || x < 0 || y < 0) return;
	if (get_z_buffer_at(x, y) < (1.0 - interpolated_reciprocal_w)) return;

	update_z_buffer_at(x, y, 1.0 - interpolated_reciprocal_w);

	draw_pixel(x, y, color);
}

void fill_triangle(triangle_t triangle, uint32_t color)
{
	int x0 = (int)triangle.points[0].x;
	int y0 = (int)triangle.points[0].y;
	float z0 = triangle.points[0].z;
	float w0 = triangle.points[0].w;

	int x1 = (int)triangle.points[1].x;
	int y1 = (int)triangle.points[1].y;
	float z1 = triangle.points[1].z;
	float w1 = triangle.points[1].w;

	int x2 = (int)triangle.points[2].x;
	int y2 = (int)triangle.points[2].y;
	float z2 = triangle.points[2].z;
	float w2 = triangle.points[2].w;

	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}


	// Draw top triangle
	float inv_slope1 = get_slope_inversed((vec2_int_t) { x0, y0 }, (vec2_int_t) { x1, y1 });
	float inv_slope2 = get_slope_inversed((vec2_int_t) { x0, y0 }, (vec2_int_t) { x2, y2 });

	for (int y = y0; y < y1; y++)
	{
		int x_start = x0 + (y - y0) * inv_slope1;
		int x_end = x0 + (y - y0) * inv_slope2;

		if (x_start > x_end)
			int_swap(&x_start, &x_end);

		for (int x = x_start; x <= x_end; x++)
		{
			draw_triangle_pixel(
				x0, y0, z0, w0,
				x1, y1, z1, w1,
				x2, y2, z2, w2,
				x, y, color
			);
		}
	}

	// Draw bottom triangle
	inv_slope1 = get_slope_inversed((vec2_int_t) { x1, y1 }, (vec2_int_t) { x2, y2 });

	for (int y = y1; y <= y2; y++)
	{
		int x_start = x1 + (y - y1) * inv_slope1;
		int x_end = x0 + (y - y0) * inv_slope2;

		if (x_start > x_end)
			int_swap(&x_start, &x_end);

		for (int x = x_start; x <= x_end; x++)
		{
			draw_triangle_pixel(
				x0, y0, z0, w0,
				x1, y1, z1, w1,
				x2, y2, z2, w2,
				x, y, color
			);
		}
	}
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	vec3_t weights = {0};
	vec2_t ba = vec2_sub(b, a);
	vec2_t ca = vec2_sub(c, a);
	vec2_t bp = vec2_sub(b, p);
	vec2_t cp = vec2_sub(c, p);
	vec2_t ap = vec2_sub(a, p);

	float area_abc = ca.x * ba.y - ca.y * ba.x;

	if (area_abc == 0.0) return weights;
	
	weights.x = (cp.x * bp.y - cp.y * bp.x) / area_abc;
	weights.y = (ap.x * cp.y - ap.y * cp.x) / area_abc;
	weights.z = 1 - weights.x - weights.y;

	return weights;
}

static void draw_texel(
	int x0, int y0, float z0, float w0, float u0, float v0, vec3_t n0,
	int x1, int y1, float z1, float w1, float u1, float v1, vec3_t n1,
	int x2, int y2, float z2, float w2, float u2, float v2, vec3_t n2,
	int x, int y, texture_t* texture, bool is_lit
) {
	vec3_t weights = barycentric_weights(
		(vec2_t) {x0, y0},
		(vec2_t) {x1, y1},
		(vec2_t) {x2, y2},
		(vec2_t) {x, y}
	);

	vec2_t uv = { 0 };
	vec3_t fragment_normal = { 0 };

	float interpolated_reciprocal_w = 1.0 / w0 * weights.x + 1.0 / w1 * weights.y + 1.0 / w2 * weights.z;
	
	if (x >= get_window_width() || y >= get_window_height() || x < 0 || y < 0) return;
	if (get_z_buffer_at(x, y) < (1.0 - interpolated_reciprocal_w)) return;
	
	update_z_buffer_at(x, y, 1.0 - interpolated_reciprocal_w);

	// Interpolate props correctly
	uv.x = u0 / w0 * weights.x + u1 / w1 * weights.y + u2 / w2 * weights.z;
	uv.y = v0 / w0 * weights.x + v1 / w1 * weights.y + v2 / w2 * weights.z;

	fragment_normal.x = n0.x / w0 * weights.x + n1.x / w1 * weights.y + n2.x / w2 * weights.z;
	fragment_normal.y = n0.y / w0 * weights.x + n1.y / w1 * weights.y + n2.y / w2 * weights.z;
	fragment_normal.z = n0.z / w0 * weights.x + n1.z / w1 * weights.y + n2.z / w2 * weights.z;

	// Clamp
	fragment_normal = interpolated_reciprocal_w != 0 ? vec3_div(fragment_normal, interpolated_reciprocal_w) : fragment_normal;
	uv = interpolated_reciprocal_w != 0 ? vec2_div(uv, interpolated_reciprocal_w) : uv;
	uv.x = uv.x < 0.0 ? 0.0 : (uv.x > 1.0 ? uv.x - floor(uv.x) : uv.x);
	uv.y = uv.y < 0.0 ? 0.0 : (uv.y > 1.0 ? uv.y - floor(uv.y) : uv.y);

	int col = uv.x * (texture->texture_width - 1);
	int row = (1 - uv.y) * (texture->texture_height - 1);

	uint32_t result_color = texture->mesh_texture[row * texture->texture_width + col];

	if (is_lit)
	{
		vec3_normalize(&fragment_normal);
		float light_intencity_factor = vec3_dot(fragment_normal, vec3_negative(get_light_view()));
		result_color = light_apply_intensity(result_color, light_intencity_factor);
	}

	draw_pixel(x, y, result_color);
}

void draw_textured_triangle(triangle_t triangle, texture_t* texture, bool is_lit)
{
	int x0 = (int)triangle.points[0].x;
	int y0 = (int)triangle.points[0].y;
	float z0 = triangle.points[0].z;
	float w0 = triangle.points[0].w;
	float u0 = triangle.texcoords[0].u;
	float v0 = triangle.texcoords[0].v;
	vec3_t n0 = triangle.normals[0];
	
	int x1 = (int)triangle.points[1].x;
	int y1 = (int)triangle.points[1].y;
	float z1 = triangle.points[1].z;
	float w1 = triangle.points[1].w;
	float u1 = triangle.texcoords[1].u;
	float v1 = triangle.texcoords[1].v;
	vec3_t n1 = triangle.normals[1];
	
	int x2 = (int)triangle.points[2].x;
	int y2 = (int)triangle.points[2].y;
	float z2 = triangle.points[2].z;
	float w2 = triangle.points[2].w;
	float u2 = triangle.texcoords[2].u;
	float v2 = triangle.texcoords[2].v;
	vec3_t n2 = triangle.normals[2];

	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
		vec3_t_swap(&n0, &n1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
		vec3_t_swap(&n1, &n2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
		vec3_t_swap(&n0, &n1);
	}


	// Draw top triangle
	float inv_slope1 = get_slope_inversed((vec2_int_t) { x0, y0 }, (vec2_int_t) { x1, y1 });
	float inv_slope2 = get_slope_inversed((vec2_int_t) { x0, y0 }, (vec2_int_t) { x2, y2 });

	for (int y = y0; y < y1; y++)
	{
		int x_start = x0 + (y - y0) * inv_slope1;
		int x_end = x0 + (y - y0) * inv_slope2;

		if (x_start > x_end) 
			int_swap(&x_start, &x_end);

		for (int x = x_start; x <= x_end; x++)
		{
			draw_texel(
				x0, y0, z0, w0, u0, v0, n0,
				x1, y1, z1, w1, u1, v1, n1,
				x2, y2, z2, w2, u2, v2, n2,
				x, y, texture, is_lit
			);
		}
	}
	
	// Draw bottom triangle
	inv_slope1 = get_slope_inversed((vec2_int_t) { x1, y1 }, (vec2_int_t) { x2, y2 });

	for (int y = y1; y <= y2; y++)
	{
		int x_start = x1 + (y - y1) * inv_slope1;
		int x_end = x0 + (y - y0) * inv_slope2;

		if (x_start > x_end) 
			int_swap(&x_start, &x_end);

		for (int x = x_start; x <= x_end; x++)
		{
			draw_texel(
				x0, y0, z0, w0, u0, v0, n0,
				x1, y1, z1, w1, u1, v1, n1,
				x2, y2, z2, w2, u2, v2, n2,
				x, y, texture, is_lit
			);
		}
	}
}
