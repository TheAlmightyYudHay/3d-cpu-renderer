#include "GlobalBuffers.h"

void FrameBuffer::SetBufferSizes(std::size_t width, std::size_t height)
{
    mWidth = width;
    mHeight = height;
    mFrameBuffer.resize(width * height);
}

void FrameBuffer::SetFrame(std::size_t x, std::size_t y, uint32_t color)
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

void ZBuffer::SetBufferSizes(std::size_t width, std::size_t height)
{
    mWidth = width;
    mHeight = height;
    mZBuffer.resize(width * height);
}

void ZBuffer::SetValue(std::size_t x, std::size_t y, float value)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return;
    mZBuffer[mWidth * y + x] = value;
}

float ZBuffer::GetValue(std::size_t x, std::size_t y)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
    {
        return 1.0;
    }
    return mZBuffer[mWidth * y + x];
}

void ZBuffer::Clear()
{
    for (auto& frame : mZBuffer)
    {
        frame = 1.0;
    }
}
