#include "Matrix2D.h"
#include "Vertex.h"

// Constructors
Matrix2D::Matrix2D(void)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			_matrix2D[i][j] = 0.0f;
		}
	}
}

Matrix2D::Matrix2D(float x1, float x2, float x3,
				   float y1, float y2, float y3,
				   float w1, float w2, float w3)
{
	_matrix2D[0][0] = x1;
	_matrix2D[0][1] = x2;
	_matrix2D[0][2] = x3;

	_matrix2D[1][0] = y1;
	_matrix2D[1][1] = y2;
	_matrix2D[1][2] = y3;

	_matrix2D[2][0] = w1;
	_matrix2D[2][1] = w2;
	_matrix2D[2][2] = w3;
}

Matrix2D::Matrix2D(const Matrix2D& matrix)
{
	Copy(matrix);
}

// Destructor
Matrix2D::~Matrix2D() {}

// Properties
float Matrix2D::GetX1(void) const {	return _matrix2D[0][0]; }
float Matrix2D::GetX2(void) const { return _matrix2D[0][1]; }
float Matrix2D::GetX3(void) const { return _matrix2D[0][2]; }

float Matrix2D::GetY1(void) const { return _matrix2D[1][0]; }
float Matrix2D::GetY2(void) const { return _matrix2D[1][1]; }
float Matrix2D::GetY3(void) const { return _matrix2D[1][2]; }

float Matrix2D::GetW1(void) const { return _matrix2D[2][0]; }
float Matrix2D::GetW2(void) const { return _matrix2D[2][1]; }
float Matrix2D::GetW3(void) const { return _matrix2D[2][2]; }


// Operator overloads
Matrix2D& Matrix2D::operator=(const Matrix2D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

const Matrix2D Matrix2D::operator*(const Matrix2D& other) const
{
	// Multiply two Matrix2Ds together and return a Matrix2D
	float tx1 = (_matrix2D[0][0] * other.GetX1()) + (_matrix2D[0][1] * other.GetY1()) + (_matrix2D[0][2] * other.GetW1());
	float tx2 = (_matrix2D[0][0] * other.GetX2()) + (_matrix2D[0][1] * other.GetY2()) + (_matrix2D[0][2] * other.GetW2());
	float tx3 = (_matrix2D[0][0] * other.GetX3()) + (_matrix2D[0][1] * other.GetY3()) + (_matrix2D[0][2] * other.GetW3());

	float ty1 = (_matrix2D[1][0] * other.GetX1()) + (_matrix2D[1][1] * other.GetY1()) + (_matrix2D[1][2] * other.GetW1());
	float ty2 = (_matrix2D[1][0] * other.GetX2()) + (_matrix2D[1][1] * other.GetY2()) + (_matrix2D[1][2] * other.GetW2());
	float ty3 = (_matrix2D[1][0] * other.GetX3()) + (_matrix2D[1][1] * other.GetY3()) + (_matrix2D[1][2] * other.GetW3());

	float tw1 = (_matrix2D[2][0] * other.GetX1()) + (_matrix2D[2][1] * other.GetY1()) + (_matrix2D[2][2] * other.GetW1());
	float tw2 = (_matrix2D[2][0] * other.GetX2()) + (_matrix2D[2][1] * other.GetY2()) + (_matrix2D[2][2] * other.GetW2());
	float tw3 = (_matrix2D[2][0] * other.GetX3()) + (_matrix2D[2][1] * other.GetY3()) + (_matrix2D[2][2] * other.GetW3());

	return Matrix2D(tx1, tx2, tx3,
					ty1, ty2, ty3,
					tw1, tw2, tw3);
}

const Vertex Matrix2D::operator*(const Vertex& p) const
{
	// Multiply a Matrix2D by a Vertex and return a Vertex
	float tx1 = _matrix2D[0][0] * p.GetX();
	float tx2 = _matrix2D[0][1] * p.GetY();
	float tx3 = _matrix2D[0][2] * p.GetW();

	float ty1 = _matrix2D[1][0] * p.GetX();
	float ty2 = _matrix2D[1][1] * p.GetY();
	float ty3 = _matrix2D[1][2] * p.GetW();

	float tw1 = _matrix2D[2][0] * p.GetX();
	float tw2 = _matrix2D[2][1] * p.GetY();
	float tw3 = _matrix2D[2][2] * p.GetW();

	// Total up each row
	float totalX = tx1 + tx2 + tx3;
	float totalY = ty1 + ty2 + ty3;
	float totalW = tw1 + tw2 + tw3;

	// Create Vertex with result
	return Vertex(totalX, totalY, 0.0f, totalW);
}

void Matrix2D::Copy(const Matrix2D& matrix)
{
	_matrix2D[0][0] = matrix.GetX1();
	_matrix2D[0][1] = matrix.GetX2();
	_matrix2D[0][2] = matrix.GetX3();

	_matrix2D[1][0] = matrix.GetY1();
	_matrix2D[1][1] = matrix.GetY2();
	_matrix2D[1][2] = matrix.GetY3();

	_matrix2D[2][0] = matrix.GetW1();
	_matrix2D[2][1] = matrix.GetW2();
	_matrix2D[2][2] = matrix.GetW3();
}
