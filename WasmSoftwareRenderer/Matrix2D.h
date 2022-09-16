#pragma once
#include "Vertex.h"

class Matrix2D
{
public:
	Matrix2D(void);
	Matrix2D(float x1, float x2, float x3,
		float y1, float y2, float y3,
		float w1, float w2, float w3);
	Matrix2D(const Matrix2D&);
	~Matrix2D();

	float GetX1(void) const;
	float GetX2(void) const;
	float GetX3(void) const;

	float GetY1(void) const;
	float GetY2(void) const;
	float GetY3(void) const;

	float GetW1(void) const;
	float GetW2(void) const;
	float GetW3(void) const;

	Matrix2D& operator= (const Matrix2D& rhs);

	const Matrix2D operator* (const Matrix2D& other) const;
	const Vertex operator* (const Vertex& p) const;

private:
	float _matrix2D[3][3];
	void Copy(const Matrix2D& matrix);
};
