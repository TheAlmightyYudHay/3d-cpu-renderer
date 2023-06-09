#include "Vector.h"
#include <cmath>

//////////////////////////////
// Vector3
//////////////////////////////

float Vector3::Length(const Vector3& v)
{
	return sqrt(Vector3::LengthSquared(v));
}

float Vector3::LengthSquared(const Vector3& v)
{
	return v.mX * v.mX + v.mY * v.mY + v.mZ + v.mZ;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{
		v1.mY * v2.mZ - v1.mZ * v2.mY,
		v1.mZ * v2.mX - v1.mX * v2.mZ,
		v1.mX * v2.mY - v1.mY * v2.mX
	};
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ;
}

Vector3 Vector3::Normalized(const Vector3& v)
{
	float factor = 1.0f / Length(v);
	return v * factor;
}

Vector3 Vector3::Negative(const Vector3& v)
{
	return Vector3{-v.mX, -v.mY, -v.mZ};
}

float Vector3::Length() const
{
	return Vector3::Length(*this);
}

float Vector3::LengthSquared() const
{
	return Vector3::LengthSquared(*this);
}

Vector3 Vector3::Normalized() const
{
	return Vector3::Normalized(*this);
}

void Vector3::Normalize()
{
	float factor = 1.0f / Length();
	mX *= factor;
	mY *= factor;
	mZ *= factor;
}

Vector4 Vector3::ToVector4() const
{
	return Vector4(mX, mY, mZ, 1);
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.mX + v2.mX, v1.mY + v2.mY, v1.mZ + v2.mZ);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.mX - v2.mX, v1.mY - v2.mY, v1.mZ - v2.mZ);
}

Vector3 operator*(const Vector3& v, float factor)
{
	return Vector3(v.mX * factor, v.mY * factor, v.mZ * factor);
}

Vector3 operator*(float factor, const Vector3& v)
{
	return v * factor;
}

Vector3 operator/(const Vector3& v, float factor)
{
	if (factor == 0.0f)
		return Vector3::Zero();

	float invFactor = 1.0f / factor;
	return v * invFactor;
}

//////////////////////////////
// Vector2
//////////////////////////////

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.mX + v2.mX, v1.mY + v2.mY);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.mX - v2.mX, v1.mY - v2.mY);
}

Vector2 operator*(const Vector2& v, float factor)
{
	return Vector2(v.mX * factor, v.mY * factor);
}

Vector2 operator*(float factor, const Vector2& v)
{
	return v * factor;
}

Vector2 operator/(const Vector2& v, float factor)
{
	float invFactor = 1.0f / factor;
	return v * invFactor;
}

float Vector2::Length(const Vector2& v)
{
	return sqrt(Vector2::LengthSquared(v));
}

float Vector2::LengthSquared(const Vector2& v)
{
	return v.mX * v.mX + v.mY * v.mY;
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.mX * v2.mX + v1.mY * v2.mY;
}

Vector2 Vector2::Normalized(const Vector2& v)
{
	float factor = 1.0f / Length(v);
	return v * factor;
}

Vector2 Vector2::Negative(const Vector2& v)
{
	return Vector2(-v.mX, -v.mY);
}

float Vector2::Length() const
{
	return Vector2::Length(*this);
}

float Vector2::LengthSquared() const
{
	return Vector2::LengthSquared(*this);
}

Vector2 Vector2::Normalized() const
{
	return Vector2::Normalized(*this);
}

void Vector2::Normalize()
{
	float factor = 1.0f / Length();
	mX *= factor;
	mY *= factor;
}

Vector4 Vector2::toVector4() const
{
	return Vector4(mX, mY, 0, 1);
}

Vector2 Vector4::ToVector2() const
{
	return Vector2(mX, mY);
}

Vector3 Vector4::ToVector3() const
{
	return Vector3(mX, mY, mZ);
}
