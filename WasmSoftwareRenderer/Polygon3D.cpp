#include <SDL.h>
#include "Polygon3D.h"

// Constructors
Polygon3D::Polygon3D(void)
{
	for (int i = 0; i < 3; i++)
	{
		_vertIndex[i] = 0;
	}

	_isBackFacing = false;
	_averageZDepth = 0.0f;
	_lightingColour = SDL_Color{ 0, 0, 0, 255 };
	_normal = Vector3D();
}

Polygon3D::Polygon3D(int indexOne, int indexTwo, int indexThree)
{
	_vertIndex[0] = indexOne;
	_vertIndex[1] = indexTwo;
	_vertIndex[2] = indexThree;
	_isBackFacing = false;
	_averageZDepth = 0.0f;
	_lightingColour = SDL_Color{ 0, 0, 0, 255 };
	_normal = Vector3D();
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

Vector3D Polygon3D::GetPolygonNormal(void) const
{
	return _normal;
}

void Polygon3D::SetPolygonNormal(const Vector3D& normal)
{
	_normal = normal;
}

float Polygon3D::GetAverageZDepth(void) const
{
	return _averageZDepth;
}

void Polygon3D::SetAverageZDepth(float averageZ)
{
	_averageZDepth = averageZ;
}

SDL_Color Polygon3D::GetLightingColour(void) const
{
	return _lightingColour;
}

void Polygon3D::SetLightingColour(const SDL_Color& color)
{
	_lightingColour = color;
}

// Public methods
void Polygon3D::SetBackFacing(bool backwards)
{
	// If this is called then the polygon is backward facing
	if (backwards)
	{
		_isBackFacing = true;
	}
	else
	{
		_isBackFacing = false;
	}
}

bool Polygon3D::DrawPolygon(void) const
{
	// If polygon is NOT backwards facing then we can draw it
	if (!_isBackFacing)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Private methods
void Polygon3D::Copy(const Polygon3D& p)
{
	for (int i = 0; i < 3; i++)
	{
		_vertIndex[i] = p.GetVertexIndex(i);
	}

	if (p.DrawPolygon())
	{
		_isBackFacing = false;
	}
	else
	{
		_isBackFacing = true;
	}

	_averageZDepth = p.GetAverageZDepth();
	_lightingColour = p.GetLightingColour();
	_normal = p.GetPolygonNormal();
}
