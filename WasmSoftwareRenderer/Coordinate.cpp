#include "Coordinate.h"

// Constructors
Coordinate::Coordinate(void)
{
	Init(0.0f, 0.0f, 0.0f, 0.0f);
}

Coordinate::Coordinate(float x, float y, float z)
{
	Init(x, y, z, 0.0f);
}

Coordinate::Coordinate(float x, float y, float z, float w)
{
	Init(x, y, z, w);
}

Coordinate::Coordinate(const Coordinate& coord)
{
	Copy(coord);
}

// Destructor
Coordinate::~Coordinate(void) {}

// Operator overloads
Coordinate& Coordinate::operator= (const Coordinate& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Properties
float Coordinate::GetX(void) const
{
	return _x;
}

void Coordinate::SetX(float x)
{
	_x = x;
}

float Coordinate::GetY(void) const
{
	return _y;
}

void Coordinate::SetY(float y)
{
	_y = y;
}

float Coordinate::GetZ(void) const
{
	return _z;
}

void Coordinate::SetZ(float z)
{
	_z = z;
}

float Coordinate::GetW(void) const
{
	return _w;
}

void Coordinate::SetW(float w)
{
	_w = w;
}

// Private methods
void Coordinate::Init(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

void Coordinate::Copy(const Coordinate& coord)
{
	_x = coord.GetX();
	_y = coord.GetY();
	_z = coord.GetZ();
	_w = coord.GetW();
}
