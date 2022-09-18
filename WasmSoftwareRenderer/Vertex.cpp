#include "Vertex.h"

// Constructors
Vertex::Vertex(void) : Coordinate() {}

Vertex::Vertex(float x, float y, float z, float w) : Coordinate(x, y, z, w) {}

Vertex::Vertex(const Vertex& v)
{
	Copy(v);
}

// Destructor
Vertex::~Vertex(void) {}

// Operator overloads
Vertex& Vertex::operator=(const Vertex& rhs)
{
	if (this != &rhs)
	{
		// only copy if we are not assigning to ourselves
		// remember that a = b is the same as a.operator=(b)
		Copy(rhs);
	}
	return *this;
}

Vector3D Vertex::operator-(const Vertex& rhs)
{
	// Operator overload to subtracts two vertices and returns a vector
	float vectX = this->GetX() - rhs.GetX();
	float vectY = this->GetY() - rhs.GetY();
	float vectZ = this->GetZ() - rhs.GetZ();

	return Vector3D(vectX, vectY, vectZ);
}

Vertex Vertex::operator+(const Vector3D& rhs)
{
	// Operator overload to add a vector to a vertex
	float x = this->GetX() + rhs.GetX();
	float y = this->GetY() + rhs.GetY();
	float z = this->GetZ() + rhs.GetZ();

	return Vertex(x, y, z, 1.0f);
}

// Public methods
Vertex& Vertex::DehomogenizeVertex()
{
	float dhX = this->GetX() / this->GetW();
	float dhY = this->GetY() / this->GetW();
	float dhZ = this->GetZ() / this->GetW();
	float dhW = this->GetW() / this->GetW();

	this->SetX(dhX);
	this->SetY(dhY);
	this->SetZ(dhZ);
	this->SetW(dhW);

	return *this;
}

// Private methods
void Vertex::Copy(const Vertex& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
	_w = v.GetW();
}
