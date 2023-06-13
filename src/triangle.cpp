#include "triangle.h"
#include <stdio.h>
#include <cmath>
#include "display.h"
#include "swap.h"
#include "light.h"
#include "configs.h"
#include "GlobalBuffers.h"
#include <climits>

int safe_pixel = 0;

float get_slope_inversed(const Vector2& from, const Vector2& to)
{
	if (static_cast<int>(to.GetY()) == static_cast<int>(from.GetY())) return 0;
	return (static_cast<int>(to.GetX()) - static_cast<int>(from.GetX())) / (float)(static_cast<int>(to.GetY()) - static_cast<int>(from.GetY()));
}

void draw_triangle_pixel(
	int x0, int y0, float z0, float w0,
	int x1, int y1, float z1, float w1,
	int x2, int y2, float z2, float w2,
	int x, int y, uint32_t color
) {
	Vector3 weights = barycentric_weights(
		Vector2 (x0, y0),
		Vector2 (x1, y1),
		Vector2 (x2, y2),
		Vector2 (x, y)
	);

	float interpolated_reciprocal_w = 1.0 / w0 * weights.GetX() + 1.0 / w1 * weights.GetY() + 1.0 / w2 * weights.GetZ();

	ZBuffer& zBuffer = GlobalBuffers::GetInstance().GetZBuffer();

	if (x >= get_window_width() || y >= get_window_height() || x < 0 || y < 0) return;
	if (zBuffer.GetValue(x, y) < (1.0 - interpolated_reciprocal_w)) return;

	zBuffer.SetValue(x, y, 1.0 - interpolated_reciprocal_w);

	GlobalBuffers::GetInstance().GetFrameBuffer().SetFrame(x, y, color);
}

void fill_triangle(triangle_t triangle, uint32_t color)
{
	int x0 = (int)triangle.points[0].GetX();
	int y0 = (int)triangle.points[0].GetY();
	float z0 = triangle.points[0].GetZ();
	float w0 = triangle.points[0].GetW();

	int x1 = (int)triangle.points[1].GetX();
	int y1 = (int)triangle.points[1].GetY();
	float z1 = triangle.points[1].GetZ();
	float w1 = triangle.points[1].GetW();

	int x2 = (int)triangle.points[2].GetX();
	int y2 = (int)triangle.points[2].GetY();
	float z2 = triangle.points[2].GetZ();
	float w2 = triangle.points[2].GetW();

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
	float inv_slope1 = get_slope_inversed(Vector2(x0, y0), Vector2(x1, y1));
	float inv_slope2 = get_slope_inversed(Vector2(x0, y0), Vector2(x2, y2));

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
	inv_slope1 = get_slope_inversed(Vector2(x1, y1), Vector2(x2, y2));

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

Vector3 barycentric_weights(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& p)
{
	Vector3 weights;
	Vector2 ba = b - a;
	Vector2 ca = c - a;
	Vector2 bp = b - p;
	Vector2 cp = c - p;
	Vector2 ap = a - p;

	float area_abc = ca.GetX() * ba.GetY() - ca.GetY() * ba.GetX();

	if (area_abc == 0.0) return weights;
	
	weights.SetX((cp.GetX() * bp.GetY() - cp.GetY() * bp.GetX()) / area_abc);
	weights.SetY((ap.GetX() * cp.GetY() - ap.GetY() * cp.GetX()) / area_abc);
	weights.SetZ(1 - weights.GetX() - weights.GetY());

	return weights;
}

static void draw_texel(
	int x0, int y0, float z0, float w0, float u0, float v0, const Vector3& n0,
	int x1, int y1, float z1, float w1, float u1, float v1, const Vector3& n1,
	int x2, int y2, float z2, float w2, float u2, float v2, const Vector3& n2,
	int x, int y, texture_t* texture, bool is_lit
) {
	Vector3 weights = barycentric_weights(
		Vector2(x0, y0),
		Vector2(x1, y1),
		Vector2(x2, y2),
		Vector2(x, y)
	);

	ZBuffer& zBuffer = GlobalBuffers::GetInstance().GetZBuffer();

	Vector2 uv;
	Vector3 fragment_normal;

	float interpolated_reciprocal_w = 1.0 / w0 * weights.GetX() + 1.0 / w1 * weights.GetY() + 1.0 / w2 * weights.GetZ();
	
	if (x >= get_window_width() || y >= get_window_height() || x < 0 || y < 0) return;
	if (zBuffer.GetValue(x, y) < (1.0 - interpolated_reciprocal_w)) return;
	
	zBuffer.SetValue(x, y, 1.0 - interpolated_reciprocal_w);

	// Interpolate props correctly
	uv.SetX(u0 / w0 * weights.GetX() + u1 / w1 * weights.GetY() + u2 / w2 * weights.GetZ());
	uv.SetY(v0 / w0 * weights.GetX() + v1 / w1 * weights.GetY() + v2 / w2 * weights.GetZ());

	fragment_normal.SetX(n0.GetX() / w0 * weights.GetX() + n1.GetX() / w1 * weights.GetY() + n2.GetX() / w2 * weights.GetZ());
	fragment_normal.SetY(n0.GetY() / w0 * weights.GetX() + n1.GetY() / w1 * weights.GetY() + n2.GetY() / w2 * weights.GetZ());
	fragment_normal.SetZ(n0.GetZ() / w0 * weights.GetX() + n1.GetZ() / w1 * weights.GetY() + n2.GetZ() / w2 * weights.GetZ());

	// Clamp
	fragment_normal = interpolated_reciprocal_w != 0 ? fragment_normal / interpolated_reciprocal_w : fragment_normal;
	uv = interpolated_reciprocal_w != 0 ? uv / interpolated_reciprocal_w : uv;
	uv.SetX(uv.GetX() < 0.0 ? 0.0 : (uv.GetX() > 1.0 ? uv.GetX() - floor(uv.GetX()) : uv.GetX()));
	uv.SetY(uv.GetY() < 0.0 ? 0.0 : (uv.GetY() > 1.0 ? uv.GetY() - floor(uv.GetY()) : uv.GetY()));

	int col = uv.GetX() * (texture->texture_width - 1);
	int row = (1 - uv.GetY()) * (texture->texture_height - 1);

	uint32_t result_color = texture->mesh_texture[row * texture->texture_width + col];

	if (is_lit)
	{
		fragment_normal.Normalize();
		float light_intencity_factor = Vector3::Dot(fragment_normal, Vector3::Negative(Configs::GetInstance().GetLight().GetViewDirection()));
		result_color = Light::ApplyIntensity(result_color, light_intencity_factor);
	}

	GlobalBuffers::GetInstance().GetFrameBuffer().SetFrame(x, y, result_color);
}

bool IsLessThanZero(float value, float epsilon = FLT_EPSILON) {
	return value < -epsilon;
}

bool IsTopLeftEdge(int x1, int y1, int x2, int y2)
{
	bool isTop = (x2 > x1) && (y1 == y2);
	bool isLeft = (y1 > y2);
	return isTop || isLeft;
}

void draw_textured_triangle(triangle_t triangle, texture_t* texture, bool is_lit)
{
	float x0 = (triangle.points[0].GetX());
	float y0 = (triangle.points[0].GetY());
	float z0 = triangle.points[0].GetZ();
	float w0 = triangle.points[0].GetW();
	float u0 = triangle.texcoords[0].u;
	float v0 = triangle.texcoords[0].v;
	Vector3 n0 = triangle.normals[0];
	
	// Since we swapped y axis in the projection matrix, we need to swap the other two points as well
	float x1 = (triangle.points[2].GetX());
	float y1 = (triangle.points[2].GetY());
	float z1 = triangle.points[2].GetZ();
	float w1 = triangle.points[2].GetW();
	float u1 = triangle.texcoords[2].u;
	float v1 = triangle.texcoords[2].v;
	Vector3 n1 = triangle.normals[2];
	
	float x2 = (triangle.points[1].GetX());
	float y2 = (triangle.points[1].GetY());
	float z2 = triangle.points[1].GetZ();
	float w2 = triangle.points[1].GetW();
	float u2 = triangle.texcoords[1].u;
	float v2 = triangle.texcoords[1].v;
	Vector3 n2 = triangle.normals[1];

	float topBound = std::floor(std::min(y0, std::min(y1, y2)));
	float bottomBound = std::ceil(std::max(y0, std::max(y1, y2)));
	float leftBound = std::floor(std::min(x0, std::min(x1, x2)));
	float rightBound = std::ceil(std::max(x0, std::max(x1, x2)));

	float dx0 = x1 - x0;
	float dx1 = x2 - x1;
	float dx2 = x0 - x2;
	float dy0 = y1 - y0;
	float dy1 = y2 - y1;
	float dy2 = y0 - y2;

	float e0V = (leftBound - x0) * dy0 - (topBound - y0) * dx0;
	float e1V = (leftBound - x1) * dy1 - (topBound - y1) * dx1;
	float e2V = (leftBound - x2) * dy2 - (topBound - y2) * dx2;

	FrameBuffer& frameBuffer = GlobalBuffers::GetInstance().GetFrameBuffer();
	ZBuffer& zBuffer = GlobalBuffers::GetInstance().GetZBuffer();
	Light& light = Configs::GetInstance().GetLight();

	float wholeAreaInversed = 1.0f / ((x2 - x0)*(y1 - y0) - (y2 - y0)*(x1 - x0));

	for (int y = topBound; y <= bottomBound; y++)
	{
		for (int x = leftBound; x <= rightBound; x++)
		{
			float e0H = e0V + dy0 * (x - leftBound);
			float e1H = e1V + dy1 * (x - leftBound);
			float e2H = e2V + dy2 * (x - leftBound);

			if (IsLessThanZero(e0H) || IsLessThanZero(e1H) || IsLessThanZero(e2H)) continue;

			float interpolated_reciprocal_w = (e0H / w2 + e1H / w0 + e2H / w1) * wholeAreaInversed;

			if (zBuffer.GetValue(x, y) < (1.0 - interpolated_reciprocal_w)) continue;

			// interpolate uv
			Vector2 uv;
			uv.SetX((u2 * e0H / w2 + u0 * e1H / w0 + u1 * e2H / w1) * wholeAreaInversed);
			uv.SetY((v2 * e0H / w2 + v0 * e1H / w0 + v1 * e2H / w1) * wholeAreaInversed);
			uv = interpolated_reciprocal_w != 0 ? uv / interpolated_reciprocal_w : uv;
			uv.SetX(uv.GetX() < 0.0 ? 0.0 : (uv.GetX() > 1.0 ? uv.GetX() - floor(uv.GetX()) : uv.GetX()));
			uv.SetY(uv.GetY() < 0.0 ? 0.0 : (uv.GetY() > 1.0 ? uv.GetY() - floor(uv.GetY()) : uv.GetY()));
			
			int col = uv.GetX() * (texture->texture_width - 1);
			int row = (1 - uv.GetY()) * (texture->texture_height - 1);

			uint32_t result_color = texture->mesh_texture[row * texture->texture_width + col];

			// interpolate normal
			Vector3 fragment_normal;
			fragment_normal.SetX((n2.GetX() * e0H / w2 + n0.GetX() * e1H / w0 + n1.GetX() * e2H / w1) * wholeAreaInversed);
			fragment_normal.SetY((n2.GetY() * e0H / w2 + n0.GetY() * e1H / w0 + n1.GetY() * e2H / w1) * wholeAreaInversed);
			fragment_normal.SetZ((n2.GetZ() * e0H / w2 + n0.GetZ() * e1H / w0 + n1.GetZ() * e2H / w1) * wholeAreaInversed);

			fragment_normal = interpolated_reciprocal_w != 0 ? fragment_normal / interpolated_reciprocal_w : fragment_normal;

			if (is_lit)
			{
				fragment_normal.Normalize();
				float light_intencity_factor = Vector3::Dot(fragment_normal, Vector3::Negative(light.GetViewDirection()));
				result_color = Light::ApplyIntensity(result_color, light_intencity_factor);
			}

			zBuffer.SetValue(x, y, 1.0 - interpolated_reciprocal_w);
			frameBuffer.SetFrame(x, y, result_color);
		}
		e0V -= dx0;
		e1V -= dx1;
		e2V -= dx2;
	}
}
