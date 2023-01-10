#pragma once
#include <Novice.h>
#define _USE_MATH_DEFINES
#include "math.h"
#include <assert.h>
#include "Vec2.h"
#include "Quad.h"

static const int kRowheight = 20;
static const int kColumnwidth = 44;

class Matrix3x3
{
public:
	/*Å@ïœêîÅ@*/
	float m[3][3];


	Matrix3x3(
		float _11 = 1.0f, float _12 = 0.0f, float _13 = 0.0f,
		float _21 = 0.0f, float _22 = 1.0f, float _23 = 0.0f,
		float _31 = 0.0f, float _32 = 0.0f, float _33 = 1.0f) {
		m[0][0] = _11, m[0][1] = _12, m[0][2] = _13;
		m[1][0] = _21, m[1][1] = _22, m[1][2] = _23;
		m[2][0] = _31, m[2][1] = _32, m[2][2] = _33;
	}

};


Matrix3x3 MakeScaleMatrix(Vec2 scale);
Matrix3x3 MakeRotateMatrix(float theta);
Matrix3x3 MakeTranslateMatrix(Vec2 translate);
Matrix3x3 MakeAffineMatrix(Vec2 scale, float rotate, Vec2 translate);

Matrix3x3 Add(Matrix3x3 matrix1, Matrix3x3 matrix2);
Matrix3x3 Subtract(Matrix3x3 matrix1, Matrix3x3 matrix2);
Matrix3x3 operator*(Matrix3x3 matrix1, Matrix3x3 matrix2);

Vec2 operator*(Vec2 vector, Matrix3x3 matrix2);
Vec2 Transform(Vec2 vector, Matrix3x3 matrix);

Quad Transform(Quad quad, Matrix3x3 matrix);

void MatrixScreenPrintf(int x, int y, Matrix3x3 matrix);

void VectorScreenPrintf(int x, int y, Vec2 vector);

void DrawWorldQuad(Vec2 LeftTop, Vec2 RightTop, Vec2 LeftBottom, Vec2 RightBottom, unsigned int color);
