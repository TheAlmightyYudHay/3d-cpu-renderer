#include "Matrix.h"
#include <cmath>

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = 0.0f;
		}
	}
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4x4 Matrix4x4::MakeScale(float sx, float sy, float sz)
{
	return Matrix4x4{
		  sx, 0.0f, 0.0f, 0.0f,
		0.0f,   sy, 0.0f, 0.0f,
		0.0f, 0.0f,   sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 Matrix4x4::MakeTranslation(float tx, float ty, float tz)
{
	return Matrix4x4{
		1.0f, 0.0f, 0.0f,   tx,
		0.0f, 1.0f, 0.0f,   ty,
		0.0f, 0.0f, 1.0f,   tz,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 Matrix4x4::MakeRotationX(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return Matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, c, -s, 0.0f,
		0.0f, s, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 Matrix4x4::MakeRotationY(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

return Matrix4x4{
		   c, 0.0f,    s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		  -s, 0.0f,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 Matrix4x4::MakeRotationZ(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return Matrix4x4{
		   c,   -s, 0.0f, 0.0f,
		   s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,    0, 1.0f
	};
}

Matrix4x4 Matrix4x4::MakePerspective(float fov, float aspect, float zNear, float zFar)
{
	float tanHalfFov = tanf(fov * 0.5f);
	float zRangeInverse = 1.0f / (zNear - zFar);

	return Matrix4x4{
		aspect/tanHalfFov,			   0.0f,               0.0f,                        0.0f,
					 0.0f, -1.0f/tanHalfFov,               0.0f,                        0.0f,
					 0.0f,             0.0f, zFar*zRangeInverse, (-zFar*zNear)*zRangeInverse,
					 0.0f,             0.0f,               1.0f,                        0.0f
	};
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = (target - eye).Normalized();
	Vector3 xaxis = Vector3::Cross(up, zaxis).Normalized();
	Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

	return Matrix4x4{
		xaxis.GetX(), xaxis.GetY(), xaxis.GetZ(), -Vector3::Dot(xaxis, eye),
		yaxis.GetX(), yaxis.GetY(), yaxis.GetZ(), -Vector3::Dot(yaxis, eye),
		zaxis.GetX(), zaxis.GetY(), zaxis.GetZ(), -Vector3::Dot(zaxis, eye),
		0.0f, 0.0f, 0.0f,                                              1.0f
	};
}

Vector4 Matrix4x4::MulVec4Project(const Matrix4x4& m, const Vector4& v)
{
	Vector4 result = MulVec4(m, v);
	float w = result.GetW();

	if (w != 0.0f)
	{
		w = 1.0f / w;

		result.SetX(result.GetX() * w);
		result.SetY(result.GetY() * w);
		result.SetZ(result.GetZ() * w);
	}

	return result;
}

Matrix4x4 Matrix4x4::MulMat4(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			for (int i = 0; i < 4; i++)
			{
				result.m[row][col] += m1.m[row][i] * m2.m[i][col];
			}
		}
	}

	return result;
}

Vector4 Matrix4x4::MulVec4(const Matrix4x4& m, const Vector4& v)
{
	return Vector4{
		m.m[0][0] * v.GetX() + m.m[0][1] * v.GetY() + m.m[0][2] * v.GetZ() + m.m[0][3] * v.GetW(),
		m.m[1][0] * v.GetX() + m.m[1][1] * v.GetY() + m.m[1][2] * v.GetZ() + m.m[1][3] * v.GetW(),
		m.m[2][0] * v.GetX() + m.m[2][1] * v.GetY() + m.m[2][2] * v.GetZ() + m.m[2][3] * v.GetW(),
		m.m[3][0] * v.GetX() + m.m[3][1] * v.GetY() + m.m[3][2] * v.GetZ() + m.m[3][3] * v.GetW()
	};
}
