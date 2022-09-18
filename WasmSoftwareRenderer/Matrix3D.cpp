#include "Matrix3D.h"
#include "Vertex.h"
#include <cmath>

// Constructors
Matrix3D::Matrix3D(void)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_matrix3D[i][j] = 0.0f;
		}
	}
}

Matrix3D::Matrix3D(float x1, float x2, float x3, float x4,
				   float y1, float y2, float y3, float y4,
				   float z1, float z2, float z3, float z4,
				   float w1, float w2, float w3, float w4)
{
	_matrix3D[0][0] = x1;
	_matrix3D[0][1] = x2;
	_matrix3D[0][2] = x3;
	_matrix3D[0][3] = x4;

	_matrix3D[1][0] = y1;
	_matrix3D[1][1] = y2;
	_matrix3D[1][2] = y3;
	_matrix3D[1][3] = y4;

	_matrix3D[2][0] = z1;
	_matrix3D[2][1] = z2;
	_matrix3D[2][2] = z3;
	_matrix3D[2][3] = z4;

	_matrix3D[3][0] = w1;
	_matrix3D[3][1] = w2;
	_matrix3D[3][2] = w3;
	_matrix3D[3][3] = w4;
}

Matrix3D::Matrix3D(const Matrix3D& matrix)
{
	Copy(matrix);
}

// Destructor
Matrix3D::~Matrix3D() {}

// Properties
float Matrix3D::GetX1(void) const {	return _matrix3D[0][0]; }
float Matrix3D::GetX2(void) const { return _matrix3D[0][1]; }
float Matrix3D::GetX3(void) const { return _matrix3D[0][2]; }
float Matrix3D::GetX4(void) const { return _matrix3D[0][3]; }

float Matrix3D::GetY1(void) const { return _matrix3D[1][0]; }
float Matrix3D::GetY2(void) const { return _matrix3D[1][1]; }
float Matrix3D::GetY3(void) const { return _matrix3D[1][2]; }
float Matrix3D::GetY4(void) const {	return _matrix3D[1][3]; }

float Matrix3D::GetZ1(void) const { return _matrix3D[2][0]; }
float Matrix3D::GetZ2(void) const { return _matrix3D[2][1]; }
float Matrix3D::GetZ3(void) const { return _matrix3D[2][2]; }
float Matrix3D::GetZ4(void) const { return _matrix3D[2][3]; }

float Matrix3D::GetW1(void) const { return _matrix3D[3][0]; }
float Matrix3D::GetW2(void) const { return _matrix3D[3][1]; }
float Matrix3D::GetW3(void) const { return _matrix3D[3][2]; }
float Matrix3D::GetW4(void) const { return _matrix3D[3][3]; }

// Operator overloads
Matrix3D& Matrix3D::operator=(const Matrix3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

const Matrix3D Matrix3D::operator*(const Matrix3D& other) const
{
	// Multiply two Matrix3Ds together and return a Matrix3D
	float tx1 = (_matrix3D[0][0] * other.GetX1()) + (_matrix3D[0][1] * other.GetY1()) + (_matrix3D[0][2] * other.GetZ1()) + (_matrix3D[0][3] * other.GetW1());
	float tx2 = (_matrix3D[0][0] * other.GetX2()) + (_matrix3D[0][1] * other.GetY2()) + (_matrix3D[0][2] * other.GetZ2()) + (_matrix3D[0][3] * other.GetW2());
	float tx3 = (_matrix3D[0][0] * other.GetX3()) + (_matrix3D[0][1] * other.GetY3()) + (_matrix3D[0][2] * other.GetZ3()) + (_matrix3D[0][3] * other.GetW3());
	float tx4 = (_matrix3D[0][0] * other.GetX4()) + (_matrix3D[0][1] * other.GetY4()) + (_matrix3D[0][2] * other.GetZ4()) + (_matrix3D[0][3] * other.GetW4());

	float ty1 = (_matrix3D[1][0] * other.GetX1()) + (_matrix3D[1][1] * other.GetY1()) + (_matrix3D[1][2] * other.GetZ1()) + (_matrix3D[1][3] * other.GetW1());
	float ty2 = (_matrix3D[1][0] * other.GetX2()) + (_matrix3D[1][1] * other.GetY2()) + (_matrix3D[1][2] * other.GetZ2()) + (_matrix3D[1][3] * other.GetW2());
	float ty3 = (_matrix3D[1][0] * other.GetX3()) + (_matrix3D[1][1] * other.GetY3()) + (_matrix3D[1][2] * other.GetZ3()) + (_matrix3D[1][3] * other.GetW3());
	float ty4 = (_matrix3D[1][0] * other.GetX4()) + (_matrix3D[1][1] * other.GetY4()) + (_matrix3D[1][2] * other.GetZ4()) + (_matrix3D[1][3] * other.GetW4());

	float tz1 = (_matrix3D[2][0] * other.GetX1()) + (_matrix3D[2][1] * other.GetY1()) + (_matrix3D[2][2] * other.GetZ1()) + (_matrix3D[2][3] * other.GetW1());
	float tz2 = (_matrix3D[2][0] * other.GetX2()) + (_matrix3D[2][1] * other.GetY2()) + (_matrix3D[2][2] * other.GetZ2()) + (_matrix3D[2][3] * other.GetW2());
	float tz3 = (_matrix3D[2][0] * other.GetX3()) + (_matrix3D[2][1] * other.GetY3()) + (_matrix3D[2][2] * other.GetZ3()) + (_matrix3D[2][3] * other.GetW3());
	float tz4 = (_matrix3D[2][0] * other.GetX4()) + (_matrix3D[2][1] * other.GetY4()) + (_matrix3D[2][2] * other.GetZ4()) + (_matrix3D[2][3] * other.GetW4());

	float tw1 = (_matrix3D[3][0] * other.GetX1()) + (_matrix3D[3][1] * other.GetY1()) + (_matrix3D[3][2] * other.GetZ1()) + (_matrix3D[3][3] * other.GetW1());
	float tw2 = (_matrix3D[3][0] * other.GetX2()) + (_matrix3D[3][1] * other.GetY2()) + (_matrix3D[3][2] * other.GetZ2()) + (_matrix3D[3][3] * other.GetW2());
	float tw3 = (_matrix3D[3][0] * other.GetX3()) + (_matrix3D[3][1] * other.GetY3()) + (_matrix3D[3][2] * other.GetZ3()) + (_matrix3D[3][3] * other.GetW3());
	float tw4 = (_matrix3D[3][0] * other.GetX4()) + (_matrix3D[3][1] * other.GetY4()) + (_matrix3D[3][2] * other.GetZ4()) + (_matrix3D[3][3] * other.GetW4());

	return Matrix3D(tx1, tx2, tx3, tx4,
					ty1, ty2, ty3, ty4,
					tz1, tz2, tz3, tz4,
					tw1, tw2, tw3, tw4);
}

const Vertex Matrix3D::operator*(const Vertex& p) const
{
	// Multiply a Matrix2D by a Vertex and return a Vertex
	float tx1 = _matrix3D[0][0] * p.GetX();
	float tx2 = _matrix3D[0][1] * p.GetY();
	float tx3 = _matrix3D[0][2] * p.GetZ();
	float tx4 = _matrix3D[0][3] * p.GetW();

	float ty1 = _matrix3D[1][0] * p.GetX();
	float ty2 = _matrix3D[1][1] * p.GetY();
	float ty3 = _matrix3D[1][2] * p.GetZ();
	float ty4 = _matrix3D[1][3] * p.GetW();

	float tz1 = _matrix3D[2][0] * p.GetX();
	float tz2 = _matrix3D[2][1] * p.GetY();
	float tz3 = _matrix3D[2][2] * p.GetZ();
	float tz4 = _matrix3D[2][3] * p.GetW();

	float tw1 = _matrix3D[3][0] * p.GetX();
	float tw2 = _matrix3D[3][1] * p.GetY();
	float tw3 = _matrix3D[3][2] * p.GetZ();
	float tw4 = _matrix3D[3][3] * p.GetW();

	// Total up each row
	float totalX = tx1 + tx2 + tx3 + tx4;
	float totalY = ty1 + ty2 + ty3 + ty4;
	float totalZ = tz1 + tz2 + tz3 + tz4;
	float totalW = tw1 + tw2 + tw3 + tw4;

	// Create Vertex with result
	return Vertex(totalX, totalY, totalZ, totalW);
}

// World transform methods
Matrix3D Matrix3D::TranslateModel(const Vector3D& translations)
{
	Matrix3D translate(1.0f, 0.0f, 0.0f, translations.GetX(),
					   0.0f, 1.0f, 0.0f, translations.GetY(),
					   0.0f, 0.0f, 1.0f, translations.GetZ(),
					   0.0f, 0.0f, 0.0f, 1.0f);

	return translate;
}

Matrix3D Matrix3D::RotateModel(const Vector3D& rotations)
{
	// Rotate around x axis
	Matrix3D rotationX(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, cos(rotations.GetX()), sin(rotations.GetX()) * -1.0f, 0.0f,
					   0.0f, sin(rotations.GetX()), cos(rotations.GetX()), 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	// Rotate around y axis
	Matrix3D rotationY(cos(rotations.GetY()), 0.0f, sin(rotations.GetY()), 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   sin(rotations.GetY()) * -1.0f, 0.0f, cos(rotations.GetY()), 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	// Rotate around z axis
	Matrix3D rotationZ(cos(rotations.GetZ()), sin(rotations.GetZ()) * -1.0f, 0.0f, 0.0f,
					   sin(rotations.GetZ()), cos(rotations.GetZ()), 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	return rotationX * rotationY * rotationZ;
}

Matrix3D Matrix3D::ScaleModel(const Vector3D& scales)
{
	Matrix3D scale(scales.GetX(), 0.0f, 0.0f, 0.0f,
				   0.0f, scales.GetY(), 0.0f, 0.0f,
				   0.0f, 0.0f, scales.GetZ(), 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);

	return scale;
}

// Private methods
void Matrix3D::Copy(const Matrix3D& matrix)
{
	_matrix3D[0][0] = matrix.GetX1();
	_matrix3D[0][1] = matrix.GetX2();
	_matrix3D[0][2] = matrix.GetX3();
	_matrix3D[0][3] = matrix.GetX4();

	_matrix3D[1][0] = matrix.GetY1();
	_matrix3D[1][1] = matrix.GetY2();
	_matrix3D[1][2] = matrix.GetY3();
	_matrix3D[1][3] = matrix.GetY4();

	_matrix3D[2][0] = matrix.GetZ1();
	_matrix3D[2][1] = matrix.GetZ2();
	_matrix3D[2][2] = matrix.GetZ3();
	_matrix3D[2][3] = matrix.GetZ4();

	_matrix3D[3][0] = matrix.GetW1();
	_matrix3D[3][1] = matrix.GetW2();
	_matrix3D[3][2] = matrix.GetW3();
	_matrix3D[3][3] = matrix.GetW4();
}
