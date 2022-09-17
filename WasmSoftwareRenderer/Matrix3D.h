#pragma once
#include "Vertex.h"

class Matrix3D
{
public:
	Matrix3D(void);
	Matrix3D(float x1, float x2, float x3, float x4,
			 float y1, float y2, float y3, float y4,
			 float z1, float z2, float z3, float z4,
			 float w1, float w2, float w3, float w4);
	Matrix3D(const Matrix3D&);
	~Matrix3D();

	float GetX1(void) const;
	float GetX2(void) const;
	float GetX3(void) const;
	float GetX4(void) const;

	float GetY1(void) const;
	float GetY2(void) const;
	float GetY3(void) const;
	float GetY4(void) const;

	float GetZ1(void) const;
	float GetZ2(void) const;
	float GetZ3(void) const;
	float GetZ4(void) const;

	float GetW1(void) const;
	float GetW2(void) const;
	float GetW3(void) const;
	float GetW4(void) const;

	Matrix3D& operator= (const Matrix3D& rhs);

	const Matrix3D operator* (const Matrix3D& other) const;
	const Vertex operator* (const Vertex& p) const;

private:
	float _matrix3D[4][4];
	void Copy(const Matrix3D& matrix);
};
