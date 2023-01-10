#include "MatVec.h"



Matrix3x3 Add(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	return {
		matrix1.m[0][0] + matrix2.m[0][0],
		matrix1.m[0][1] + matrix2.m[0][1],
		matrix1.m[0][2] + matrix2.m[0][2],
		matrix1.m[1][0] + matrix2.m[1][0],
		matrix1.m[1][1] + matrix2.m[1][1],
		matrix1.m[1][2] + matrix2.m[1][2],
		matrix1.m[2][0] + matrix2.m[2][0],
		matrix1.m[2][1] + matrix2.m[2][1],
		matrix1.m[2][2] + matrix2.m[2][2]
	};
}

Matrix3x3 Subtract(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	return {
		matrix1.m[0][0] - matrix2.m[0][0],
		matrix1.m[0][1] - matrix2.m[0][1],
		matrix1.m[0][2] - matrix2.m[0][2],
		matrix1.m[1][0] - matrix2.m[1][0],
		matrix1.m[1][1] - matrix2.m[1][1],
		matrix1.m[1][2] - matrix2.m[1][2],
		matrix1.m[2][0] - matrix2.m[2][0],
		matrix1.m[2][1] - matrix2.m[2][1],
		matrix1.m[2][2] - matrix2.m[2][2]
	};
}

Matrix3x3 operator*(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	return {
		matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0],
		matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1],
		matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2],
		matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0],
		matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1],
		matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2],
		matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0],
		matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1],
		matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2]
	};
}

Matrix3x3 MakeScaleMatrix(Vec2 scale) {
	return {
			scale.x, 0, 0,
			0, scale.y, 0,
			0, 0, 1
	};
}

Matrix3x3 MakeRotateMatrix(float theta) {
	float s = sinf(theta);
	float c = cosf(theta);
	return {
			c,		s,		0,
			-s,		c,		0,
			0,		0,		1
	};
}

Matrix3x3 MakeTranslateMatrix(Vec2 translate) {
	return {
			1, 0, 0,
			0, 1, 0,
			translate.x, translate.y, 1
	};
}

Matrix3x3 MakeAffineMatrix(Vec2 scale, float rotate, Vec2 translate) {
	float s = sinf(rotate);
	float c = cosf(rotate);
	return{
		scale.x * c,  scale.x * s, 0,
		scale.y * -s, scale.y * c, 0,
		translate.x, translate.y, 1
	};
}



Vec2 operator*(Vec2 vector, Matrix3x3 matrix) {
	return{
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1]
	};
}

Vec2 Transform(Vec2 vector, Matrix3x3 matrix) {
	Vec2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Quad Transform(Quad quad, Matrix3x3 matrix) {
	Quad result;
	result.LeftTop.x = quad.LeftTop.x * matrix.m[0][0] + quad.LeftTop.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.LeftTop.y = quad.LeftTop.x * matrix.m[0][1] + quad.LeftTop.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	result.RightTop.x = quad.RightTop.x * matrix.m[0][0] + quad.RightTop.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.RightTop.y = quad.RightTop.x * matrix.m[0][1] + quad.RightTop.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	result.LeftBottom.x = quad.LeftBottom.x * matrix.m[0][0] + quad.LeftBottom.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.LeftBottom.y = quad.LeftBottom.x * matrix.m[0][1] + quad.LeftBottom.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	result.RightBottom.x = quad.RightBottom.x * matrix.m[0][0] + quad.RightBottom.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.RightBottom.y = quad.RightBottom.x * matrix.m[0][1] + quad.RightBottom.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = quad.LeftTop.x * matrix.m[0][2] + quad.LeftTop.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	float x = quad.RightTop.x * matrix.m[0][2] + quad.RightTop.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	float y = quad.LeftBottom.x * matrix.m[0][2] + quad.LeftBottom.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	float z = quad.RightBottom.x * matrix.m[0][2] + quad.RightBottom.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.LeftTop.x /= w;
	result.LeftTop.y /= w;
	assert(x != 0.0f);
	result.RightTop.x /= x;
	result.RightTop.y /= x;
	assert(y != 0.0f);
	result.LeftBottom.x /= y;
	result.LeftBottom.y /= y;
	assert(z != 0.0f);
	result.RightBottom.x /= z;
	result.RightBottom.y /= z;
	return result;
}



void MatrixScreenPrintf(int x, int y, Matrix3x3 matrix) {
	for (int row = 0; row < 2; row++){
		for (int column = 0; column < 2; column++){
			Novice::ScreenPrintf(x + column * kColumnwidth, y + row * kRowheight, "%.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, Vec2 vector) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnwidth, y, "%.02f", vector.y);
}

//ê—p•`‰æŠÖ”
void DrawWorldQuad(Vec2 LeftTop, Vec2 RightTop, Vec2 LeftBottom, Vec2 RightBottom, unsigned int color) {
	float x1 = LeftTop.x;
	float y1 = LeftTop.y * -1 + 500;
	float x2 = RightTop.x;
	float y2 = RightTop.y * -1 + 500;
	float x3 = LeftBottom.x;
	float y3 = LeftBottom.y * -1 + 500;
	float x4 = RightBottom.x;
	float y4 = RightBottom.y * -1 + 500;
	Novice::DrawQuad((int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3, (int)x4, (int)y4, 0, 0, 0, 0, 0, color);
}