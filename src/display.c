#include "display.h"
#include <stdio.h>
#include <math.h>
#include "vector.h"

////////////////////////////////////////////////////////////////////////
// RENDERING MODES:
//////////////////////////////////////////////////////////////////////// 
const int wireframe_mask = 1;
const int vertices_mask = 2;
const int filled_mask = 4;
const int backface_culling_mask = 8;
//////////////////////////////////////////////////////////////////////// 

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t* color_buffer = NULL;
int window_width = 800;
int window_height = 600;
extern const int pixelGridSize = 100;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window)
	{
		fprintf(stderr, "Error creating SDL window\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer\n");
		return false;
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void render_color_buffer(void)
{
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(sizeof(uint32_t) * window_width)
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color)
{
	for (int row = 0; row < window_height; row++)
	{
		for (int col = 0; col < window_width; col++)
		{
			color_buffer[window_width * row + col] = color;
		}
	}
}

//void draw_grid(uint32_t gridColor)
//{
//	for (int row = 0; row < window_height; row++)
//	{
//		const int rowOffset = window_width * row;
//
//		if (row % pixelGridSize == 0)
//		{
//			for (int col = 0; col < window_width; col += 1)
//			{
//				color_buffer[rowOffset + col] = gridColor;
//			}
//		}
//		else
//		{
//			for (int col = 0; col < window_width; col += pixelGridSize)
//			{
//				color_buffer[rowOffset + col] = gridColor;
//			}
//		}
//	}
//}

void draw_grid(uint32_t gridColor)
{
	for (int row = 0; row < window_height; row += pixelGridSize)
	{
		for (int col = 0; col < window_width; col += pixelGridSize)
		{
			color_buffer[row * window_width + col] = gridColor;
		}
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int row = y; row < y + height; row++)
	{
		for (int col = x; col < x + width; col++)
		{
			draw_pixel(col, row, color);
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if (x >= window_width || y >= window_height || x < 0 || y < 0) return;
	color_buffer[window_width * y + x] = color;
}

void destroy_window(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	if (y0 == y1)
	{
		draw_line_horisontal(x0, x1, y0, color);
	}
	else
	{
		draw_line_bresenham(x0, y0, x1, y1, color);
	}
}

void draw_line_horisontal(x0, x1, y, color)
{
	for (int xCurr = x0; xCurr <= x1; xCurr++)
	{
		draw_pixel(xCurr, y, color);
	}
}

void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color)
{
	int dX = x1 - x0;
	int dY = y1 - y0;

	int longest_delta = abs(dX) > abs(dY) ? abs(dX) : abs(dY);

	float xStep = dX / (float)longest_delta;
	float yStep = dY / (float)longest_delta;

	float xCurr = x0;
	float yCurr = y0;

	for (int i = 0; i <= longest_delta; i++)
	{
		draw_pixel(round(xCurr), round(yCurr), color);
		xCurr += xStep;
		yCurr += yStep;
	}
}

void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color)
{
	int dX = abs(x1 - x0);
	int dY = abs(y1 - y0);
	int yStep = y1 - y0 > 0 ? 1 : -1;
	int xStep = x1 - x0 > 0 ? 1 : -1;

	if (dX > dY)
	{
		int D = 2 * dY - dX;

		while (x0 != x1)
		{
			draw_pixel(x0, y0, color);

			if (D > 0)
			{
				y0 += yStep;
				D -= 2 * dX;
			}

			D += 2 * dY;
			x0 += xStep;
		}
	}
	else
	{
		int D = 2 * dX - dY;

		while (y0 != y1)
		{
			draw_pixel(x0, y0, color);

			if (D > 0)
			{
				x0 += xStep;
				D -= 2 * dY;
			}

			D += 2 * dX;
			y0 += yStep;
		}
	}
}

void draw_wireframe(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}