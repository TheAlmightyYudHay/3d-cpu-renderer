#include <cmath>
#include "Vector.h"

//////////////////////////////
// Vector3
//////////////////////////////

float Vector3::Length(const Vector3& v)
{
	return sqrt(Vector3::LengthSquared(v));
}

float Vector3::LengthSquared(const Vector3& v)
{
	return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ;
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

void Vector3::Swap(Vector3& v1, Vector3& v2)
{
	Vector3 temp = v1;
	v1 = v2;
	v2 = temp;
}

Vector3::Vector3(const Vector4& v) : mX(v.GetX()), mY(v.GetY()), mZ(v.GetZ())
{
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

Vector2::Vector2(const Vector4& v) : mX(v.GetX()), mY(v.GetY())
{
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
