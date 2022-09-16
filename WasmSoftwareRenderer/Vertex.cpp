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

// Private methods
void Vertex::Copy(const Vertex& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
	_w = v.GetW();
}
