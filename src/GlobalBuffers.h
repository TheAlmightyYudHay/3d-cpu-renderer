#pragma once

#include "mesh.h"
#include <stdint.h>
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer() : mWidth{ 0 }, mHeight{ 0 } {}
	void SetBufferSizes(int width, int height);
	void SetFrame(int x, int y, uint32_t color);
	void Clear(uint32_t color);

	uint32_t* GetBuffer() { return mFrameBuffer.data(); }

private:
	int mWidth;
	int mHeight;
	std::vector<uint32_t> mFrameBuffer;
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

private:
	MeshContainer mMeshContainer;
	FrameBuffer mFrameBuffer;

	// Should be private to avoid unwanted access.
	GlobalBuffers() = default;

	GlobalBuffers(const GlobalBuffers&) = delete;
	GlobalBuffers& operator=(const GlobalBuffers&) = delete;
	GlobalBuffers(const GlobalBuffers&&) = delete;
};

