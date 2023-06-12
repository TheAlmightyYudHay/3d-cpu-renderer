#pragma once

#include "mesh.h"

class GlobalBuffers
{
public:
	static GlobalBuffers& GetInstance() {
		static GlobalBuffers instance;
		return instance;
	}

	MeshContainer& GetMeshContainer() { return mMeshContainer; }

private:
	MeshContainer mMeshContainer;

	// Should be private to avoid unwanted access.
	GlobalBuffers() = default;

	GlobalBuffers(const GlobalBuffers&) = delete;
	GlobalBuffers& operator=(const GlobalBuffers&) = delete;
	GlobalBuffers(const GlobalBuffers&&) = delete;
};

