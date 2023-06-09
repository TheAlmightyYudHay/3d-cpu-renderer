#pragma once

class Vector2;
class Vector3;
class Vector4;


///////////////////////////////////////////
// Vector3
///////////////////////////////////////////

class Vector3
{
public:
	static Vector3 Zero() { return Vector3(0, 0, 0); }

	static float Length(const Vector3& v);
	static float LengthSquared(const Vector3& v);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Normalized(const Vector3& v);
	static Vector3 Negative(const Vector3& v);

	Vector3() : mX(0), mY(0), mZ(0) {}	
	Vector3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

	float Length() const;
	float LengthSquared() const;
	Vector3 Normalized() const;
	void Normalize();

	friend Vector3 operator+ (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator- (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator* (const Vector3& v, float factor);
	friend Vector3 operator* (float factor, const Vector3& v);
	friend Vector3 operator/ (const Vector3& v, float factor);

	float GetX() const { return mX; }
	float GetY() const { return mY; }
	float GetZ() const { return mZ; }

	Vector4 ToVector4() const;

private:
	float mX, mY, mZ;
};

///////////////////////////////////////////
// Vector2
///////////////////////////////////////////

class Vector2
{
public:
	static Vector2 Zero() { return Vector2(0, 0); }

	static float Length(const Vector2& v);
	static float LengthSquared(const Vector2& v);
	static float Dot(const Vector2& v1, const Vector2& v2);
	static Vector2 Normalized(const Vector2& v);
	static Vector2 Negative(const Vector2& v);

	Vector2() : mX(0), mY(0) {}
	Vector2(float x, float y) : mX(x), mY(y) {}

	float Length() const;
	float LengthSquared() const;
	Vector2 Normalized() const;
	void Normalize();

	friend Vector2 operator+ (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator- (const Vector2& v1, const Vector2& v2);
	friend Vector2 operator* (const Vector2& v, float factor);
	friend Vector2 operator* (float factor, const Vector2& v);
	friend Vector2 operator/ (const Vector2& v, float factor);

	float GetX() const { return mX; }
	float GetY() const { return mY; }

	Vector4 toVector4() const;

private:
	float mX, mY;
};

///////////////////////////////////////////
// Vector4
///////////////////////////////////////////

class Vector4
{
public:
	Vector4(float x, float y, float z, float w = 1) : mX(x), mY(y), mZ(z), mW(w) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }
	float GetZ() const { return mZ; }
	float GetW() const { return mW; }

	void SetX(float x) { mX = x; }
	void SetY(float y) { mY = y; }
	void SetZ(float z) { mZ = z; }
	void SetW(float w) { mW = w; }

	Vector2 ToVector2() const;
	Vector3 ToVector3() const;
private:
	float mX, mY, mZ, mW;
};

