#pragma once

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

////////////////////////////////////////////////////////////////////////
// RENDERING MODES:
//////////////////////////////////////////////////////////////////////// 
// 1 - wireframe and vertices (mask 0011)
// 2 - wireframe (mask 0001)
// 3 - filled triangles (mask 0100)
// 4 - filled triangles and wireframe (mask 0101)
//////////////////////////////////////////////////////////////////////// 
// c - backface culling enabled (flag & 8 == 1)
// d - backface culling disabled (~flag & 8 == 0)
////////////////////////////////////////////////////////////////////////

extern const int wireframe_mask;
extern const int vertices_mask;
extern const int filled_mask;
extern const int backface_culling_mask;

////////////////////////////////////////////////////////////////////////

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
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_horisontal(int x0, int x1, int y, uint32_t color);
void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color);
void draw_wireframe(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void destroy_window(void);