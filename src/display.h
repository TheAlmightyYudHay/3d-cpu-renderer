#pragma once

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;

extern uint32_t* color_buffer;
extern int window_width;
extern int window_height;
extern const int pixelGridSize;

bool initialize_window(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void draw_grid(uint32_t gridColor);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void destroy_window(void);