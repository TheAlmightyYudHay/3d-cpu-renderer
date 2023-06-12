#pragma once

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

int get_window_width(void);
int get_window_height(void);
SDL_Renderer* get_renderer(void);
float* get_z_buffer(void);
float get_z_buffer_at(int x, int y);
void update_z_buffer_at(int x, int y, float value);
uint32_t* get_color_buffer(void);
bool initialize_window(void);
void render_color_buffer(void);
void clear_z_buffer(void);
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