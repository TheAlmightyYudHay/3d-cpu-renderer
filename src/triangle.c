#include "triangle.h"
#include <stdio.h>
#include <math.h>
#include "display.h"

float get_slope_inversed(vec2_int_t p1, vec2_int_t p2)
{
	if (p2.y == p1.y) return 0;
	return (p2.x - p1.x) / (float)(p2.y - p1.y);
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

	float xLeftCurr = pTop.x - 1;
	float xRightCurr = pTop.x + 1;

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

	float xLeftCurr = leftBase.x - 1;
	float xRightCurr = rightBase.x + 1;

	for (int yCurr = leftBase.y; yCurr <= pBot.y; yCurr++)
	{
		draw_line(floor(xLeftCurr), yCurr, ceil(xRightCurr), yCurr, color);
		xLeftCurr += leftStep;
		xRightCurr += rightStep;
	}
}

void int_swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
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

	float slope = (pMax.y - pMin.y) / (float)(pMax.x - pMin.x);

	int mX = pMin.x + (pMid.y - pMin.y) / slope;
	int mY = pMid.y;
	vec2_int_t pDiv = { .x = mX, .y = mY };

	draw_triangle_top(pMin, pMid, pDiv, color);
	draw_triangle_bottom(pMax, pMid, pDiv, color);
}