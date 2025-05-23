#define _USE_MATH_DEFINES
#include <Novice.h>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <imgui.h>

#include <algorithm>

const char kWindowTitle[] = "LE2B_07_クリタ_ユウダイ_タイトル";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;


struct Vector3
{
	float x, y, z;
};

struct Matrix4x4
{
	float m[4][4];
};

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

// 00_01
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
	return { scalar * v.x, scalar * v.y, scalar * v.z };
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// 長さ(ノルム)
float Length(const Vector3& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

// 正規化
Vector3 Normalize(const Vector3& v)
{
	float length = Length(v);

	return { v.x / length, v.y / length, v.z / length };
}

// 3次元ベクトルの数値表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// 00_02
// 1. 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	m3 =
	{
		m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3],
		m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1], m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3],
		m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],
		m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1], m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3],
	};
	return m3;
}

// 2. 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	m3 =
	{
		m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3],
		m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1], m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3],
		m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],
		m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1], m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3],
	};
	return m3;
}

// 3. 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	float factor1[4], factor2[4] = { 0.0f };
	for (int row = 0; row < 4; ++row)
	{
		factor1[0] = m1.m[row][0];
		factor1[1] = m1.m[row][1];
		factor1[2] = m1.m[row][2];
		factor1[3] = m1.m[row][3];
		for (int column = 0; column < 4; ++column)
		{
			factor2[0] = m2.m[0][column];
			factor2[1] = m2.m[1][column];
			factor2[2] = m2.m[2][column];
			factor2[3] = m2.m[3][column];
			m3.m[row][column]
				= factor1[0] * factor2[0] + factor1[1] * factor2[1] + factor1[2] * factor2[2] + factor1[3] * factor2[3];
		}
	}
	return m3;
}

// 4. 逆行列
Matrix4x4 Inverse(const Matrix4x4& a)
{
	Matrix4x4 inverseM;
	inverseM.m[0][0]
		= (a.m[1][1] * a.m[2][2] * a.m[3][3]) + (a.m[1][2] * a.m[2][3] * a.m[3][1]) + (a.m[1][3] * a.m[2][1] * a.m[3][2])
		- (a.m[1][3] * a.m[2][2] * a.m[3][1]) - (a.m[1][2] * a.m[2][1] * a.m[3][3]) - (a.m[1][1] * a.m[2][3] * a.m[3][2]);
	inverseM.m[0][1]
		= -(a.m[0][1] * a.m[2][2] * a.m[3][3]) - (a.m[0][2] * a.m[2][3] * a.m[3][1]) - (a.m[0][3] * a.m[2][1] * a.m[3][2])
		+ (a.m[0][3] * a.m[2][2] * a.m[3][1]) + (a.m[0][2] * a.m[2][1] * a.m[3][3]) + (a.m[0][1] * a.m[2][3] * a.m[3][2]);
	inverseM.m[0][2]
		= (a.m[0][1] * a.m[1][2] * a.m[3][3]) + (a.m[0][2] * a.m[1][3] * a.m[3][1]) + (a.m[0][3] * a.m[1][1] * a.m[3][2])
		- (a.m[0][3] * a.m[1][2] * a.m[3][1]) - (a.m[0][2] * a.m[1][1] * a.m[3][3]) - (a.m[0][1] * a.m[1][3] * a.m[3][2]);
	inverseM.m[0][1]
		= -(a.m[0][1] * a.m[1][2] * a.m[2][3]) - (a.m[0][2] * a.m[1][3] * a.m[2][1]) - (a.m[0][3] * a.m[1][1] * a.m[2][2])
		+ (a.m[0][3] * a.m[1][2] * a.m[2][1]) + (a.m[0][2] * a.m[1][1] * a.m[2][3]) + (a.m[0][1] * a.m[1][3] * a.m[2][2]);

	inverseM.m[1][0]
		= -(a.m[1][0] * a.m[2][2] * a.m[3][3]) - (a.m[1][2] * a.m[2][3] * a.m[3][0]) - (a.m[1][3] * a.m[2][0] * a.m[3][2])
		+ (a.m[1][3] * a.m[2][2] * a.m[3][0]) + (a.m[1][2] * a.m[2][0] * a.m[3][3]) + (a.m[1][0] * a.m[2][3] * a.m[3][2]);
	inverseM.m[1][1]
		= (a.m[0][0] * a.m[2][2] * a.m[3][3]) + (a.m[0][2] * a.m[2][3] * a.m[3][0]) + (a.m[0][3] * a.m[2][0] * a.m[3][2])
		- (a.m[0][3] * a.m[2][2] * a.m[3][0]) - (a.m[0][2] * a.m[2][0] * a.m[3][3]) - (a.m[0][0] * a.m[2][3] * a.m[3][2]);
	inverseM.m[1][2]
		= -(a.m[0][0] * a.m[1][2] * a.m[3][3]) - (a.m[0][2] * a.m[1][3] * a.m[3][0]) - (a.m[0][3] * a.m[1][0] * a.m[3][2])
		+ (a.m[0][3] * a.m[1][2] * a.m[3][0]) + (a.m[0][2] * a.m[1][0] * a.m[3][3]) + (a.m[0][0] * a.m[1][3] * a.m[3][2]);
	inverseM.m[1][3]
		= (a.m[0][0] * a.m[1][2] * a.m[2][3]) + (a.m[0][2] * a.m[1][3] * a.m[2][0]) + (a.m[0][3] * a.m[1][0] * a.m[2][2])
		- (a.m[0][3] * a.m[1][2] * a.m[2][0]) - (a.m[0][2] * a.m[1][0] * a.m[2][3]) - (a.m[0][0] * a.m[1][3] * a.m[2][2]);

	inverseM.m[2][0]
		= (a.m[1][0] * a.m[2][1] * a.m[3][3]) + (a.m[1][1] * a.m[2][3] * a.m[3][0]) + (a.m[1][3] * a.m[2][0] * a.m[3][1])
		- (a.m[1][3] * a.m[2][1] * a.m[3][0]) - (a.m[1][1] * a.m[2][0] * a.m[3][3]) - (a.m[1][0] * a.m[2][3] * a.m[3][1]);
	inverseM.m[2][1]
		= -(a.m[0][0] * a.m[2][1] * a.m[3][3]) - (a.m[0][1] * a.m[2][3] * a.m[3][0]) - (a.m[0][3] * a.m[2][0] * a.m[3][1])
		+ (a.m[0][3] * a.m[2][1] * a.m[3][0]) + (a.m[0][1] * a.m[2][0] * a.m[3][3]) + (a.m[0][0] * a.m[2][3] * a.m[3][1]);
	inverseM.m[2][2]
		= (a.m[0][0] * a.m[1][1] * a.m[3][3]) + (a.m[0][1] * a.m[1][3] * a.m[3][0]) + (a.m[0][3] * a.m[1][0] * a.m[3][1])
		- (a.m[0][3] * a.m[1][1] * a.m[3][0]) - (a.m[0][1] * a.m[1][0] * a.m[3][3]) - (a.m[0][0] * a.m[1][3] * a.m[3][1]);
	inverseM.m[2][3]
		= -(a.m[0][0] * a.m[1][1] * a.m[2][3]) - (a.m[0][1] * a.m[1][3] * a.m[2][0]) - (a.m[0][3] * a.m[1][0] * a.m[2][1])
		+ (a.m[0][3] * a.m[1][1] * a.m[2][0]) + (a.m[0][1] * a.m[1][0] * a.m[2][3]) + (a.m[0][0] * a.m[1][3] * a.m[2][1]);

	inverseM.m[3][0]
		= -(a.m[1][0] * a.m[2][1] * a.m[3][2]) - (a.m[1][1] * a.m[2][2] * a.m[3][0]) - (a.m[1][2] * a.m[2][0] * a.m[3][1])
		+ (a.m[1][2] * a.m[2][1] * a.m[3][0]) + (a.m[1][1] * a.m[2][0] * a.m[3][2]) + (a.m[1][0] * a.m[2][2] * a.m[3][1]);
	inverseM.m[3][1]
		= (a.m[0][0] * a.m[2][1] * a.m[3][2]) + (a.m[0][1] * a.m[2][2] * a.m[3][0]) + (a.m[0][2] * a.m[2][0] * a.m[3][1])
		- (a.m[0][2] * a.m[2][1] * a.m[3][0]) - (a.m[0][1] * a.m[2][0] * a.m[3][2]) - (a.m[0][0] * a.m[2][2] * a.m[3][1]);
	inverseM.m[3][2]
		= -(a.m[0][0] * a.m[1][1] * a.m[3][2]) - (a.m[0][1] * a.m[1][2] * a.m[3][0]) - (a.m[0][2] * a.m[1][0] * a.m[3][1])
		+ (a.m[0][2] * a.m[1][1] * a.m[3][0]) + (a.m[0][1] * a.m[1][0] * a.m[3][2]) + (a.m[0][0] * a.m[1][2] * a.m[3][1]);
	inverseM.m[3][3]
		= (a.m[0][0] * a.m[1][1] * a.m[2][2]) + (a.m[0][1] * a.m[1][2] * a.m[2][0]) + (a.m[0][2] * a.m[1][0] * a.m[2][1])
		- (a.m[0][2] * a.m[1][1] * a.m[2][0]) - (a.m[0][1] * a.m[1][0] * a.m[2][2]) - (a.m[0][0] * a.m[1][2] * a.m[2][1]);

	float A =
		a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2]
		- a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1] - a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2]
		- a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3] - a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1] - a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2]
		+ a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1] + a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2]
		+ a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2]
		- a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1] - a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2]
		- a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0] - a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0] - a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0]
		+ a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0] + a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseM.m[i][j] *= 1 / A;
		}
	}

	return inverseM;
}

// 5. 転置行列
Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 m1 = { 0.0f };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m1.m[i][j] = m.m[j][i];
		}
	}

	return m1;
}

// 6. 単位行列の作成
Matrix4x4 MakeIdentity4x4()
{
	return { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
}

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]
			);
		}
	}
}

// 00_03
// 1. 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	return
	{
		1.0f,		 0.0f,		  0.0f,		   0.0f,
		0.0f,		 1.0f,		  0.0f,		   0.0f,
		0.0f,		 0.0f,		  1.0f,		   0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
}

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	return
	{
		scale.x,	0.0f,		0.0f,		0.0f,
		0.0f,		scale.y,	0.0f,		0.0f,
		0.0f,		0.0f,		scale.z,	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};
}

// 3. 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0]
		+ vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1]
		+ vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2]
		+ vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float  w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3]
		+ vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// 00_04
// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	return
	{
		 1.0f,	 0.0f,			   0.0f,			 0.0f,
		 0.0f,   std::cos(radian), std::sin(radian), 0.0f,
		 0.0f,	-std::sin(radian), std::cos(radian), 0.0f,
		 0.0f,	 0.0f,			   0.0f,			 1.0f
	};
}

// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	return
	{
		 std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		 0.0f,			   1.0f,  0.0f,				0.0f,
		 std::sin(radian), 0.0f,  std::cos(radian), 0.0f,
		 0.0f,			   0.0f,  0.0f,				1.0f
	};
}

// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	return
	{
		 std::cos(radian), std::sin(radian), 0.0f, 0.0f,
		-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
		 0.0f,			   0.0f,			 1.0f, 0.0f,
		 0.0f,			   0.0f,			 0.0f, 1.0f
	};
}

//// 00_05
//// 3次元アフィン変換行列
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
//{
//	return Matrix4x4
//	{
//
//		scale.x * (std::cos(rotate.y) * std::cos(rotate.z)),
//		scale.x * (std::cos(rotate.y) * std::sin(rotate.z)),
//		scale.x * (-std::sin(rotate.y)),
//		0.0f,
//
//		scale.y * (-std::cos(rotate.x) * std::sin(rotate.z) + std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z)),
//		scale.y * (std::cos(rotate.x) * std::cos(rotate.z) + std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z)),
//		scale.y * (std::sin(rotate.x) * std::cos(rotate.y)),
//		0.0f,
//
//		scale.z * (std::sin(rotate.x) * std::sin(rotate.z) + std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z)),
//		scale.z * (-std::sin(rotate.x) * std::cos(rotate.z) + std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z)),
//		scale.z * (std::cos(rotate.x) * std::cos(rotate.y)),
//		0.0f,
//
//		translate.x,
//		translate.y,
//		translate.z,
//		1.0f
//	};
//
//}

// 3次元アフィン変換行列（Y → X → Z）
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateMatrix = Multiply(MakeRotateZMatrix(rotate.z),
		Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)));

	return Multiply(Multiply(MakeScaleMatrix(scale), rotateMatrix), MakeTranslateMatrix(translate));
}

// 3次元アフィン変換行列(GPT製)（Y → X → Z）
Matrix4x4 MakeAffineMatrixB(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	float cx = cosf(rotate.x), sx = sinf(rotate.x);
	float cy = cosf(rotate.y), sy = sinf(rotate.y);
	float cz = cosf(rotate.z), sz = sinf(rotate.z);

	Matrix4x4 m;

	m.m[0][0] = scale.x * (cz * cy + sz * sx * sy);
	m.m[0][1] = scale.x * (sz * cx);
	m.m[0][2] = scale.x * (cz * -sy + sz * sx * cy);
	m.m[0][3] = 0.0f;

	m.m[1][0] = scale.y * (-sz * cy + cz * sx * sy);
	m.m[1][1] = scale.y * (cz * cx);
	m.m[1][2] = scale.y * (-sz * -sy + cz * sx * cy);
	m.m[1][3] = 0.0f;

	m.m[2][0] = scale.z * (cx * sy);
	m.m[2][1] = scale.z * (-sx);
	m.m[2][2] = scale.z * (cx * cy);
	m.m[2][3] = 0.0f;

	m.m[3][0] = translate.x;
	m.m[3][1] = translate.y;
	m.m[3][2] = translate.z;
	m.m[3][3] = 1.0f;

	return m;
}

// 3次元アフィン変換行列（FPSカメラ用）(Y → X)
Matrix4x4 MakeAffineMatrixFPS(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateMatrix = Multiply(
		MakeRotateXMatrix(rotate.x),  // Pitch（上下）
		MakeRotateYMatrix(rotate.y)   // Yaw（左右）
	);
	return Multiply(Multiply(MakeScaleMatrix(scale), rotateMatrix), MakeTranslateMatrix(translate));
}

// 01_00

// cot（コタンジェント）関数が無いので作る
float cot(float theta)
{
	return 1.0f / std::tan(theta);
}

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	return
	{
		(1.0f / aspectRatio) * cot(fovY / 2.0f), 0.0f, 0.0f, 0.0f,
		0.0f, cot(fovY / 2.0f), 0.0f, 0.0f,
		0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, (-nearClip * farClip) / (farClip - nearClip), 0.0f,
	};
}

// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	return
	{
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f
	};
}

// 3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	return
	{
		width / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -height / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, maxDepth - minDepth, 0.0f,
		left + (width / 2.0f), top + (height / 2.0f), minDepth, 1.0f
	};
}

// 01_01
// クロス積関数
Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

/// <summary>
/// 指定した座標を中心とする正三角形の頂点を計算する関数
/// </summary>
/// <param name="center">正三角形の中心座標</param>
/// <param name="sideLength">正三角形の辺の長さ</param>
/// <param name="vertices">正三角形の頂点を格納する配列</param>
void MakeRegularTriangle(const Vector3& center, float sideLength, Vector3 vertices[3])
{
	// 正三角形の高さを計算
	float height = std::sqrt(3.0f) / 2.0f * sideLength;

	// 頂点の計算
	vertices[0] = { center.x, center.y + 2.0f / 3.0f * height, center.z }; // 上の頂点
	vertices[1] = { center.x - sideLength / 2.0f, center.y - 1.0f / 3.0f * height, center.z }; // 左下の頂点
	vertices[2] = { center.x + sideLength / 2.0f, center.y - 1.0f / 3.0f * height, center.z }; // 右下の頂点
}

void RotateAngleCorrection(float& rotate)
{
	if (rotate > float(M_PI))
	{
		rotate = rotate - 2.0f * float(M_PI);
	}
	if (rotate < -float(M_PI))
	{
		rotate = rotate + 2.0f * float(M_PI);
	}
}

// 01_02
// Gridを表示する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 0.5f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10;										// 分割数												
	//const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	// 1つ分の長さ
	Vector3 start, end, screenStart, screenEnd;
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		start = { kGridHalfWidth * (xIndex - kSubdivision / 2.0f), 0.0f, kGridHalfWidth * kSubdivision / 2.0f };
		end = { kGridHalfWidth * (xIndex - kSubdivision / 2.0f), 0.0f, -kGridHalfWidth * kSubdivision / 2.0f };
		screenStart = Transform(start, viewProjectionMatrix);
		screenEnd = Transform(end, viewProjectionMatrix);
		screenStart = Transform(screenStart, viewportMatrix);
		screenEnd = Transform(screenEnd, viewportMatrix);
		if (xIndex != kSubdivision / 2.0f)
		{
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
	// 左から右への線も順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		start = { kGridHalfWidth * kSubdivision / 2.0f, 0.0f, kGridHalfWidth * (zIndex - kSubdivision / 2.0f) };
		end = { -kGridHalfWidth * kSubdivision / 2.0f, 0.0f, kGridHalfWidth * (zIndex - kSubdivision / 2.0f) };
		screenStart = Transform(start, viewProjectionMatrix);
		screenEnd = Transform(end, viewProjectionMatrix);
		screenStart = Transform(screenStart, viewportMatrix);
		screenEnd = Transform(screenEnd, viewportMatrix);
		if (zIndex != kSubdivision / 2.0f)
		{
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
	start = { 0.0f, 0.0f, kGridHalfWidth * kSubdivision / 2.0f };
	end = { 0.0f, 0.0f, -kGridHalfWidth * kSubdivision / 2.0f };
	screenStart = Transform(start, viewProjectionMatrix);
	screenEnd = Transform(end, viewProjectionMatrix);
	screenStart = Transform(screenStart, viewportMatrix);
	screenEnd = Transform(screenEnd, viewportMatrix);
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);

	start = { kGridHalfWidth * kSubdivision / 2.0f, 0.0f, 0.0f };
	end = { -kGridHalfWidth * kSubdivision / 2.0f, 0.0f, 0.0f };
	screenStart = Transform(start, viewProjectionMatrix);
	screenEnd = Transform(end, viewProjectionMatrix);
	screenStart = Transform(screenStart, viewportMatrix);
	screenEnd = Transform(screenEnd, viewportMatrix);
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);
}

// Sphere構造体
struct Sphere
{
	Vector3 center;	//!< 中心座標
	float radius;	//!< 半径
};

// Sphereを表示する関数
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color)
{
	const uint32_t kLatSubdivision = 10;	// 緯度方向の分割数
	const uint32_t kLonSubdivision = 20;	// 経度方向の分割数

	const float kLonEvery = static_cast<float>(M_PI) / kLatSubdivision;			// 経度分割1つ分の角度
	const float kLatEvery = 2.0f * static_cast<float>(M_PI) / kLonSubdivision;		// 緯度分割1つ分の角度
	float lat, lon;						// 緯度、経度
	Vector3 a, b, c;					// 描画用座標	
	Vector3 screenA, screenB, screenC;
	// 緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kLatSubdivision; ++latIndex)
	{
		lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex;	// 現在の緯度
		// 経度の方向に分割　0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kLonSubdivision; ++lonIndex)
		{
			lon = lonIndex * kLonEvery;						// 現在の経度
			// World座標系でのa、b、cを求める
			a = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon)
			};

			b = {
				sphere.center.x + sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon)
			};

			c = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery)
			};

			// a、b、cをScreen座標まで変換
			screenA = Transform(a, viewProjectionMatrix);
			screenA = Transform(screenA, viewportMatrix);
			screenB = Transform(b, viewProjectionMatrix);
			screenB = Transform(screenB, viewportMatrix);
			screenC = Transform(c, viewProjectionMatrix);
			screenC = Transform(screenC, viewportMatrix);
			// ab、bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

// 直線
struct Line
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

// 半直線
struct Ray
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

// 線分
struct Segment
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	// a・b / ||b||²
	float t = Dot(v1, v2) / powf(Length(v2), 2);	

	return
	{
		t * v2.x,
		t * v2.y,
		t * v2.z,
	};
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{

	Vector3 projBA = Project(Subtract(point, segment.origin), segment.diff);
	
	return Add(segment.origin, projBA);
}

// 球の当たり判定を求める関数
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2つの球の中心点間の距離を求める
	float distance = Length(Subtract(sphere2.center, sphere1.center));
	// 半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zAxis = Normalize(Subtract(target, eye));   // 前方向
	Vector3 xAxis = Normalize(Cross(up, zAxis));         // 右方向
	Vector3 yAxis = Cross(zAxis, xAxis);                 // 上方向

	Matrix4x4 view{};
	view.m[0][0] = xAxis.x; view.m[0][1] = yAxis.x; view.m[0][2] = zAxis.x; view.m[0][3] = 0.0f;
	view.m[1][0] = xAxis.y; view.m[1][1] = yAxis.y; view.m[1][2] = zAxis.y; view.m[1][3] = 0.0f;
	view.m[2][0] = xAxis.z; view.m[2][1] = yAxis.z; view.m[2][2] = zAxis.z; view.m[2][3] = 0.0f;
	view.m[3][0] = -Dot(xAxis, eye); view.m[3][1] = -Dot(yAxis, eye); view.m[3][2] = -Dot(zAxis, eye); view.m[3][3] = 1.0f;
	return view;
}

struct Plane
{
	Vector3 normal;	//!< 法線
	float distance;	//!< 距離
};

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 平面と球の中心点間の距離を求める
	float distance = Dot(plane.normal, sphere.center) - plane.distance;
	// 半径よりも短ければ衝突
	if (std::abs(distance) <= sphere.radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//Vector3 center = Multiply(plane.distance, plane.normal);	// 1
	//Vector3 perpendiculars[4];
	//perpendiculars[0] = Normalize(Perpendicular(plane.normal));	// 2
	//perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };	// 3
	//perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);	// 4
	//perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };	// 5
	//// 6
	//Vector3 points[4];
	//for (int32_t index = 0; index < 4; ++index)
	//{
	//	Vector3 extend = Multiply(2.0f, perpendiculars[index]);
	//	Vector3 point = Add(center, extend);
	//	points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);

	//	
	//	switch (index)
	//	{
	//		case 0:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, RED);
	//			break;

	//		case 1:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, BLUE);
	//			break;

	//		case 2:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, GREEN);
	//			break;

	//		case 3:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, BLACK);
	//			break;
	//	}
	//}

	// 中心から ±u ±v を組み合わせた４点を取る
	Vector3 N = Normalize(plane.normal);              // 法線はまず正規化
	Vector3 U = Normalize(Perpendicular(N));          // 平面内の１方向
	Vector3 V = Normalize(Cross(N, U));               // U と直交するもう１方向

	float halfSize = 2.0f;
	Vector3 center = Multiply(plane.distance, N);      // 正規化済み N と distance で中心
	Vector3 point[4] = 
	{
		Add(Add(center, Multiply(halfSize, U)), Multiply(halfSize, V)),  // +u +v
		Add(Add(center, Multiply(-halfSize, U)), Multiply(halfSize, V)),  // -u +v
		Add(Add(center, Multiply(-halfSize, U)), Multiply(-halfSize, V)),  // -u -v
		Add(Add(center, Multiply(halfSize, U)), Multiply(-halfSize, V))   // +u -v
	};

	DrawSphere({ point[0], 0.03f }, viewProjectionMatrix, viewportMatrix, RED);
	DrawSphere({ point[1], 0.03f }, viewProjectionMatrix, viewportMatrix, BLUE);
	DrawSphere({ point[2], 0.03f }, viewProjectionMatrix, viewportMatrix, GREEN);
	DrawSphere({ point[3], 0.03f }, viewProjectionMatrix, viewportMatrix, BLACK);
	
	Vector3 points[4];
		

	for (int32_t index = 0; index < 4; ++index)
	{
		points[index] = Transform(Transform(point[index], viewProjectionMatrix), viewportMatrix);
	}

	for (int32_t index = 0; index < 4; ++index)
	{
		Novice::DrawLine(
			static_cast<int>(points[index].x), static_cast<int>(points[index].y),
			static_cast<int>(points[(index + 1) % 4].x), static_cast<int>(points[(index + 1) % 4].y), color);

	}

	
		
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Segment segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
	Vector3 point{ -1.5f, 0.6f, 0.6f };
	// pointを線分に射影したベクトル。今回は正しく計算出来ているかを確認するためだけに使う
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

	// この値が線分上の点を表す
	Vector3 closestPoint = ClosestPoint(point, segment);


	// 球の描画

	Sphere sphere;
	sphere.center = { 0.0f, 0.0f, 0.0f };
	sphere.radius = 0.5f;
	
	Plane plane;
	plane.normal = { 0.0f, 1.0f, 0.0f };
	plane.distance = 1.0f;
	
	

	Vector3 cameraPos{ 0.0f, 0.0f, 0.0f };
	Vector3 cameraSize{ 1.0f, 1.0f, 1.0f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };


	/*Matrix4x4 cameraMatrix;*/
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewportMatrix;

	// main関数の直前あたりに追加
	
	float radius = 6.0f;    // 原点からの距離
	const float rotationSpeed = 0.005f; // ドラッグ速度の調整係数

	float theta = 0.0f;
	float phi = 0.0f;

	// マウス座標を取得
	int mouseX, mouseY;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Novice::GetMousePosition(&mouseX, &mouseY);

		///
		/// ↓更新処理ここから
		///

		if (Novice::IsPressMouse(2) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			// ウィンドウ上ではない領域でのドラッグ
			ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle, 0.0f);

			// カメラ角度に反映
			phi += delta.x * rotationSpeed;
			theta += delta.y * rotationSpeed;

			if (phi > static_cast<float>(M_PI))
			{
				phi = phi - 2.0f * static_cast<float>(M_PI);
			}
			else if (phi < -static_cast<float>(M_PI))
			{
				phi = phi + 2.0f * static_cast<float>(M_PI);
			}

			// ピッチを上下90°未満にクランプ
			const float limit = 0.47f * static_cast<float>(M_PI); // 約85°
			theta = std::clamp(theta, -limit, limit);
			
			ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
		}

		int wheel = Novice::GetWheel(); // 前フレームとの差分
		radius -= wheel * 0.005f;              // 感度は 0.1f などで調整
		radius = max(1.0f, min(radius, 20.0f)); // クランプして近づきすぎ防止

		cameraRotate.x = phi;
		cameraRotate.y = theta;

		cameraTranslate.x = radius * std::cos(theta) * std::sin(phi);
		cameraTranslate.y = radius * std::sin(theta);
		cameraTranslate.z = radius * std::cos(theta) * std::cos(phi);

		ImGui::Begin("Window");
		ImGui::DragFloat3("sphere.pos", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("plane.distance", &plane.distance, 0.01f);
		ImGui::InputFloat3("CameraRotate", &cameraRotate.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		plane.normal = Normalize(plane.normal);	// 法線ベクトルを正規化

		// ビュー行列を生成
			
		
		viewMatrix = MakeLookAtMatrix(cameraTranslate, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

		projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		// VPMatrixを作成
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		// ViewportMatrixを作成
		viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth),
			static_cast<float>(kWindowHeight), 0.0f, 1.0f);



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		
		if (IsCollision(sphere, plane))
		{
			DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, RED);
		}
		else
		{
			DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);
		


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}