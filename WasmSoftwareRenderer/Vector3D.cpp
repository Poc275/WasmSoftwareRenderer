#include "Vector3D.h"
#include <cmath>

// Constructors
Vector3D::Vector3D(void) {}

Vector3D::Vector3D(float x, float y, float z) : Coordinate(x, y, z) {}

Vector3D::Vector3D(const Vector3D& v)
{
	Copy(v);
}

// Desctructor
Vector3D::~Vector3D(void) {}

// Operator overloads
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Dot product using the multiplication operator overload
float Vector3D::operator*(const Vector3D& other)
{
	return this->GetX() * other.GetX() + this->GetY() * other.GetY() + this->GetZ() * other.GetZ();
}

// Vector addition for Gouraud shading
Vector3D Vector3D::operator+(const Vector3D& rhs)
{
	float x = this->GetX() + rhs.GetX();
	float y = this->GetY() + rhs.GetY();
	float z = this->GetZ() + rhs.GetZ();

	return Vector3D(x, y, z);
}

// Vector division by a float for Gouraud shading
Vector3D Vector3D::operator/(const float rhs)
{
	float x = this->GetX() / rhs;
	float y = this->GetY() / rhs;
	float z = this->GetZ() / rhs;

	return Vector3D(x, y, z);
}

Vector3D Vector3D::operator-(const Vector3D& rhs)
{
	float x = this->GetX() - rhs.GetX();
	float y = this->GetY() - rhs.GetY();
	float z = this->GetZ() - rhs.GetZ();

	return Vector3D(x, y, z);
}

// Public methods
Vector3D Vector3D::CrossProduct(const Vector3D& other)
{
	float cpX = other.GetY() * this->GetZ() - other.GetZ() * this->GetY();
	float cpY = other.GetZ() * this->GetX() - other.GetX() * this->GetZ();
	float cpZ = other.GetX() * this->GetY() - other.GetY() * this->GetX();

	return Vector3D(cpX, cpY, cpZ);
}

Vector3D& Vector3D::Normalise()
{
	float normX = this->GetX() * this->GetX();
	float normY = this->GetY() * this->GetY();
	float normZ = this->GetZ() * this->GetZ();

	float length = sqrt(normX + normY + normZ);

	this->SetX(this->GetX() / length);
	this->SetY(this->GetY() / length);
	this->SetZ(this->GetZ() / length);

	return *this;
}

float Vector3D::GetVectorLength() const
{
	float xSq = this->GetX() * this->GetX();
	float ySq = this->GetY() * this->GetY();
	float zSq = this->GetZ() * this->GetZ();

	return sqrt(xSq + ySq + zSq);
}

Vector3D& Vector3D::ScaleVector(float scalar)
{
	this->SetX(this->GetX() * scalar);
	this->SetY(this->GetY() * scalar);
	this->SetZ(this->GetZ() * scalar);

	return *this;
}

// Private methods
void Vector3D::Copy(const Vector3D& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
}
