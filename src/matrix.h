#pragma once

#include "Vector.h"

class Matrix4x4
{
public:
	static Matrix4x4 Identity();
	static Matrix4x4 MakeScale(float sx, float sy, float sz);
	static Matrix4x4 MakeTranslation(float tx, float ty, float tz);
	static Matrix4x4 MakeRotationX(float angle);
	static Matrix4x4 MakeRotationY(float angle);
	static Matrix4x4 MakeRotationZ(float angle);
	static Matrix4x4 MakePerspective(float fov, float aspect, float zNear, float zFar);
	static Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
	static Matrix4x4 MulMat4(const Matrix4x4& m1, const Matrix4x4& m2);
	static Vector4 MulVec4Project(const Matrix4x4& m, const Vector4& v);
	static Vector4 MulVec4(const Matrix4x4& m, const Vector4& v);

	Matrix4x4();
	Matrix4x4(float m00, float m01, float m02, float m03,
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33);
	


private:
	float m[4][4];
};

