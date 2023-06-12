#pragma once

#include "mesh.h"
#include <stdint.h>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer() : mWidth{ 0 }, mHeight{ 0 } {}
	void SetBufferSizes(std::size_t width, std::size_t height);
	void SetFrame(std::size_t x, std::size_t y, uint32_t color);
	void Clear(uint32_t color);

	uint32_t* GetBuffer() { return mFrameBuffer.data(); }

private:
	std::size_t mWidth;
	std::size_t mHeight;
	std::vector<uint32_t> mFrameBuffer;
};

class ZBuffer
{
public:
	ZBuffer() : mWidth{ 0 }, mHeight{ 0 } {}
	void SetBufferSizes(std::size_t width, std::size_t height);
	void SetValue(std::size_t x, std::size_t y, float value);
	float GetValue(std::size_t x, std::size_t y);
	void Clear();

	float* GetBuffer() { return mZBuffer.data(); }
private:
	std::size_t mWidth;
	std::size_t mHeight;
	std::vector<float> mZBuffer;
};

class GlobalBuffers
{
public:
	static GlobalBuffers& GetInstance() {
		static GlobalBuffers instance;
		return instance;
	}

	MeshContainer& GetMeshContainer() { return mMeshContainer; }
	FrameBuffer& GetFrameBuffer() { return mFrameBuffer; }
	ZBuffer& GetZBuffer() { return mZBuffer; }

private:
	MeshContainer mMeshContainer;
	FrameBuffer mFrameBuffer;
	ZBuffer mZBuffer;

	// Should be private to avoid unwanted access.
	GlobalBuffers() = default;

	GlobalBuffers(const GlobalBuffers&) = delete;
	GlobalBuffers& operator=(const GlobalBuffers&) = delete;
	GlobalBuffers(const GlobalBuffers&&) = delete;
};

