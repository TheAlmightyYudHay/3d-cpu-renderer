#include "display.h"
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t* color_buffer = NULL;
int window_width = 800;
int window_height = 600;
extern const int pixelGridSize = 10;

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
	for (int row = 0; row < window_height; row += 10)
	{
		for (int col = 0; col < window_width; col += 10)
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
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}