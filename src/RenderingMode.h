#pragma once

#include <map>
#include <bitset>

////////////////////////////////////////////////////////////////////////
// RENDERING MODES:
//////////////////////////////////////////////////////////////////////// 
// 1 - wireframe and vertices (mask 0011)
// 2 - wireframe (mask 0001)
// 3 - filled triangles (mask 0100)
// 4 - filled triangles and wireframe (mask 0101)
// 5 - textured (mask 10000)
// 6 - textured and wireframe (mask 10001)
//////////////////////////////////////////////////////////////////////// 
// c - backface culling enabled (flag & 8 == 1)
// d - backface culling disabled (~flag & 8 == 0)
// l - lighting enabled (flag & 32 == 1)
////////////////////////////////////////////////////////////////////////

class RenderingMode
{
public:
	enum class Mode
	{
		WIREFRAME = 0,
		VERTICES,
		FILLED,
		BACKFACE_CULLING,
		TEXTURED,
		LIGHTING
	};

	static std::map<Mode, uint8_t> Modes;

	RenderingMode() : mRenderingMode{0} {}
	void SetDefault();

	void SetMode(Mode mode) { mRenderingMode.set(Modes[mode]); }
	void UnsetMode(Mode mode) { mRenderingMode.reset(Modes[mode]); }
	void ToggleMode(Mode mode) { mRenderingMode.flip(Modes[mode]); }
	void Reset() { mRenderingMode.reset(); }
	bool IsSet(Mode mode) const { return mRenderingMode.test(Modes[mode]); }

private:
	std::bitset<8> mRenderingMode;
};

