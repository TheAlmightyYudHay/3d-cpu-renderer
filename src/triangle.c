#include "triangle.h"
#include <stdio.h>
#include <math.h>
#include "display.h"
#include "swap.h"

int safe_pixel = 0;

float get_slope_inversed(vec2_int_t from, vec2_int_t to)
{
	if (to.y == from.y) return 0;
	return (to.x - from.x) / (float)(to.y - from.y);
}

void draw_triangle_top(vec2_int_t pTop, vec2_int_t p2, vec2_int_t p3, uint32_t color)
{
	if (pTop.y == p3.y || pTop.y == p2.y) return;

	vec2_int_t leftBase = p2;
	vec2_int_t rightBase = p3;

	if (p3.x < p2.x)
	{
		leftBase = p3;
		rightBase = p2;
	}

	float leftStep = get_slope_inversed(pTop, leftBase);
	float rightStep = get_slope_inversed(pTop, rightBase);

	float xLeftCurr = pTop.x - safe_pixel;
	float xRightCurr = pTop.x + safe_pixel;

	for (int yCurr = pTop.y; yCurr < leftBase.y; yCurr++)
	{
		draw_line(floor(xLeftCurr), yCurr, ceil(xRightCurr), yCurr, color);
		xLeftCurr += leftStep;
		xRightCurr += rightStep;
	}
}

void draw_triangle_bottom(vec2_int_t pBot, vec2_int_t p2, vec2_int_t p3, uint32_t color)
{
	if (pBot.y == p3.y || pBot.y == p2.y) return;

	vec2_int_t leftBase = p2;
	vec2_int_t rightBase = p3;

	if (p3.x < p2.x)
	{
		leftBase = p3;
		rightBase = p2;
	}

	float leftStep = get_slope_inversed(leftBase, pBot);
	float rightStep = get_slope_inversed(rightBase, pBot);

	float xLeftCurr = leftBase.x - safe_pixel;
	float xRightCurr = rightBase.x + safe_pixel;

	for (int yCurr = leftBase.y; yCurr <= pBot.y; yCurr++)
	{
		draw_line(floor(xLeftCurr), yCurr, ceil(xRightCurr), yCurr, color);
		xLeftCurr += leftStep;
		xRightCurr += rightStep;
	}
}

void fill_triangle(triangle_t triangle, uint32_t color)
{
	int x0 = (int)triangle.points[0].x;
	int y0 = (int)triangle.points[0].y;
	int x1 = (int)triangle.points[1].x;
	int y1 = (int)triangle.points[1].y;
	int x2 = (int)triangle.points[2].x;
	int y2 = (int)triangle.points[2].y;

	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	vec2_int_t pMin = { x0, y0 };
	vec2_int_t pMid = { x1, y1 };
	vec2_int_t pMax = { x2, y2 };

	int mX = 0;
	int mY = pMid.y;

	if (pMax.x - pMin.x == 0)
	{
		mX = pMax.x;
	}
	else 
	{
		float slope = (pMax.y - pMin.y) / (float)(pMax.x - pMin.x);
		mX = pMin.x + (pMid.y - pMin.y) / slope;
	}

	vec2_int_t pDiv = { .x = mX, .y = mY };

	draw_triangle_top(pMin, pMid, pDiv, color);
	draw_triangle_bottom(pMax, pMid, pDiv, color);

}

void draw_textured_triangle_top(
	vec2_int_t pTop, float uTop, float vTop,
	vec2_int_t p2, float u2, float v2, 
	vec2_int_t p3, float u3, float v3,
	uint32_t* texture
) {
	if (pTop.y == p3.y || pTop.y == p2.y) return;

	vec2_int_t leftBase = p2;
	vec2_int_t rightBase = p3;

	if (p3.x < p2.x)
	{
		leftBase = p3;
		rightBase = p2;
	}

	float leftStep = get_slope_inversed(pTop, leftBase);
	float rightStep = get_slope_inversed(pTop, rightBase);

	float xLeftCurr = pTop.x - safe_pixel;
	float xRightCurr = pTop.x + safe_pixel;

	for (int yCurr = pTop.y; yCurr < leftBase.y; yCurr++)
	{
		for (int xCurr = floor(xLeftCurr); xCurr <= ceil(xRightCurr); xCurr++)
		{
			uint32_t pixel_color = 0xFFFFFFFF;
			draw_pixel(xCurr, yCurr, pixel_color);
		}
		xLeftCurr += leftStep;
		xRightCurr += rightStep;
	}
}

void draw_textured_triangle_bottom(
	vec2_int_t pBot, float uBot, float vBot,
	vec2_int_t p2, float u2, float v2,
	vec2_int_t p3, float u3, float v3,
	uint32_t* texture
) {
	if (pBot.y == p3.y || pBot.y == p2.y) return;

	vec2_int_t leftBase = p2;
	vec2_int_t rightBase = p3;

	if (p3.x < p2.x)
	{
		leftBase = p3;
		rightBase = p2;
	}

	float leftStep = get_slope_inversed(leftBase, pBot);
	float rightStep = get_slope_inversed(rightBase, pBot);

	float xLeftCurr = leftBase.x - safe_pixel;
	float xRightCurr = rightBase.x + safe_pixel;

	for (int yCurr = leftBase.y; yCurr <= pBot.y; yCurr++)
	{
		for (int xCurr = floor(xLeftCurr); xCurr <= ceil(xRightCurr); xCurr++)
		{
			uint32_t pixel_color = 0xFFFFFFFF;
			draw_pixel(xCurr, yCurr, pixel_color);
		}
		xLeftCurr += leftStep;
		xRightCurr += rightStep;
	}
}

void draw_textured_triangle(triangle_t triangle, uint32_t* texture)
{
	int x0 = (int)triangle.points[0].x;
	int y0 = (int)triangle.points[0].y;
	int u0 = triangle.texcoords[0].u;
	int v0 = triangle.texcoords[0].v;
	int x1 = (int)triangle.points[1].x;
	int y1 = (int)triangle.points[1].y;
	int u1 = triangle.texcoords[1].u;
	int v1 = triangle.texcoords[1].v;
	int x2 = (int)triangle.points[2].x;
	int y2 = (int)triangle.points[2].y;
	int u2 = triangle.texcoords[2].u;
	int v2 = triangle.texcoords[2].v;

	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	/*vec2_int_t pMin = { x0, y0 };
	vec2_int_t pMid = { x1, y1 };
	vec2_int_t pMax = { x2, y2 };

	int mX = 0;
	int mY = pMid.y;

	float uDiv = 0;
	float vDiv = 0;

	if (pMax.x == pMin.x)
	{
		mX = pMax.x;
	}
	else
	{
		float slope = (pMax.y - pMin.y) / (float)(pMax.x - pMin.x);
		mX = pMin.x + (pMid.y - pMin.y) / slope;
	}

	vec2_int_t pDiv = { .x = mX, .y = mY };

	draw_textured_triangle_top(pMin, u0, v0, pMid, u1, v1, pDiv, uDiv, vDiv, texture);
	draw_textured_triangle_bottom(pMax, u2, v2, pMid, u1, v1, pDiv, uDiv, vDiv, texture);*/


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
			////
			draw_pixel(x, y, 0xFFFFFFFF);
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
			////
			draw_pixel(x, y, 0xFFFFFFFF);
		}
	}
}
