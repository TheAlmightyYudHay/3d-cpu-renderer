#include "display.h"
#include <stdio.h>
#include <math.h>
#include "vector.h"

////////////////////////////////////////////////////////////////////////
// RENDERING MODES:
//////////////////////////////////////////////////////////////////////// 
extern const int wireframe_mask = 1;
extern const int vertices_mask = 2;
extern const int filled_mask = 4;
extern const int backface_culling_mask = 8;
extern const int textured_mask = 16;
extern const int lighting_mask = 32;
//////////////////////////////////////////////////////////////////////// 

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* color_buffer_texture = NULL;

static uint32_t* color_buffer = NULL;
static float* z_buffer = NULL;
static int window_width = 320;
static int window_height = 200;
static const int pixelGridSize = 10;

int get_window_width(void) { return window_width; }
int get_window_height(void) { return window_height; }
SDL_Renderer* get_renderer(void) { return renderer; }
float* get_z_buffer(void) { return z_buffer; }
uint32_t* get_color_buffer(void) { return color_buffer; }

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	int fullscreen_window_width = display_mode.w;
	int fullscreen_window_height = display_mode.h;

	window_width = fullscreen_window_width / 4;
	window_height = fullscreen_window_height / 4;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		fullscreen_window_width,
		fullscreen_window_height,
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

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	return true;
}

void render_color_buffer(void)
{
	SDL_RenderPresent(renderer);
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(sizeof(uint32_t) * window_width)
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_z_buffer(void)
{
	for (int i = 0; i < window_height * window_width; i++)
	{
		z_buffer[i] = 1.0;
	}
}

void clear_color_buffer(uint32_t color)
{
	for (int i = 0; i < window_height * window_width; i++)
	{
		color_buffer[i] = color;
	}
}

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

float get_z_buffer_at(int x, int y)
{
	if (x >= window_width || y >= window_height || x < 0 || y < 0)
	{
		return 1.0;
	}
	return z_buffer[(window_width)*y + x];
}

void update_z_buffer_at(int x, int y, float value)
{
	if (x >= window_width || y >= window_height || x < 0 || y < 0)
	{
		return;
	}
	return z_buffer[(window_width)*y + x] = value;
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
	if (x < 0 || x >= window_width || y < 0 || y >= window_height) return;
	color_buffer[window_width * y + x] = color;
}

void destroy_window(void)
{
	free(color_buffer);
	free(z_buffer);
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

void draw_line_horisontal(int x0, int x1, int y, uint32_t color)
{
	int xMin = x0 < x1 ? x0 : x1;
	int xMax = x0 > x1 ? x0 : x1;
	for (int xCurr = xMin; xCurr <= xMax; xCurr++)
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