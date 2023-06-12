#include "GlobalBuffers.h"

void FrameBuffer::SetBufferSizes(int width, int height)
{
    mWidth = width;
    mHeight = height;
    mFrameBuffer.resize(width * height);
}

void FrameBuffer::SetFrame(int x, int y, uint32_t color)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return;
    mFrameBuffer[mWidth * y + x] = color;
}

void FrameBuffer::Clear(uint32_t color)
{
    for (auto& frame : mFrameBuffer)
	{
		frame = color;
	}
}
