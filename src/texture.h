#pragma once

#include <stdint.h>
#include "upng.h"

typedef struct {
	float u;
	float v;
} tex2_t;

typedef struct {
	int texture_width;
	int texture_height;
	uint32_t* mesh_texture;
	upng_t* png_texture;
} texture_t;

extern const uint8_t REDBRICK_TEXTURE[];

texture_t load_png_texture_data(const char* filepath);
void free_texture(texture_t* texture);
