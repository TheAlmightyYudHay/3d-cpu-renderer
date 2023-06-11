#include "light.h"

static light_t light = {
	/*.direction =*/ { 0, -0.5, 1 },
	/*.view =*/ { 0, -1, 0 }
};

Vector3 get_light_direction(void) { return light.direction; }
Vector3 get_light_view(void) { return light.view; }
void set_view_light(const Vector3& value) { light.view = value; }
void normalize_light_directon(void) { light.direction.Normalize(); }

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor)
{
	if (percentage_factor <= 0.0) return 0xFF000000;
	uint32_t a = (original_color & 0xFF000000);
	uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
	uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
	uint32_t b = (original_color & 0x000000FF) * percentage_factor;

	return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}