#include "Polygon3D.h"

// Constructors
Polygon3D::Polygon3D(void)
{
	for (int i = 0; i < 3; i++)
	{
		_vertIndex[i] = 0;
	}
}

Polygon3D::Polygon3D(int indexOne, int indexTwo, int indexThree)
{
	_vertIndex[0] = indexOne;
	_vertIndex[1] = indexTwo;
	_vertIndex[2] = indexThree;
}

Polygon3D::Polygon3D(const Polygon3D& p)
{
	Copy(p);
}

// Destructor
Polygon3D::~Polygon3D() {}

// Operator overloads
Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Properties
int Polygon3D::GetVertexIndex(int index) const
{
	return _vertIndex[index];
}

// Private methods
void Polygon3D::Copy(const Polygon3D& p)
{
	for (int i = 0; i < 3; i++)
	{
		_vertIndex[i] = p.GetVertexIndex(i);
	}
}
