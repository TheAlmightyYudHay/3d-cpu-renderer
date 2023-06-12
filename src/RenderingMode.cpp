#include "RenderingMode.h"

std::map<RenderingMode::Mode, uint8_t> RenderingMode::Modes = {
	{RenderingMode::Mode::WIREFRAME, 0},
	{RenderingMode::Mode::VERTICES, 1},
	{RenderingMode::Mode::FILLED, 2},
	{RenderingMode::Mode::BACKFACE_CULLING, 3},
	{RenderingMode::Mode::TEXTURED, 4},
	{RenderingMode::Mode::LIGHTING, 5}
};

void RenderingMode::SetDefault()
{
	SetMode(Mode::WIREFRAME);
	SetMode(Mode::BACKFACE_CULLING);
}
