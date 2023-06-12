#pragma once

#include "Vector.h"
#include <stdint.h>

class Light {
public:
	static uint32_t ApplyIntensity(uint32_t original_color, float percentage_factor);
	
	Light(const Vector3& direction, const Vector3& view) : mDirection(direction.Normalized()), mViewDirection(view.Normalized()) {}

	Vector3 GetDirection() const { return mDirection; }
	Vector3 GetViewDirection() const { return mViewDirection; }
	void SetDirection(const Vector3& direction) { mDirection = direction.Normalized(); }
	void SetView(const Vector3& view) { mViewDirection = view.Normalized(); }

private:
	Vector3 mDirection;
	Vector3 mViewDirection;
};