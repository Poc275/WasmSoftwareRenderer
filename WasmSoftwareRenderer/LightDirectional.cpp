#include "LightDirectional.h"

// Constructors
LightDirectional::LightDirectional(void) : Light()
{
	_directionToLightSource = Vector3D();
}

LightDirectional::LightDirectional(float redIntensity, float greenIntensity, float blueIntensity, Vector3D direction)
	: Light(redIntensity, greenIntensity, blueIntensity)
{
	_directionToLightSource = direction;
	// Direction vector needs to be normalised
	_directionToLightSource.Normalise();
}

LightDirectional::LightDirectional(const LightDirectional& ld)
{
	Copy(ld);
}

// Destructor
LightDirectional::~LightDirectional(void) {}

// Properties
Vector3D LightDirectional::GetDirectionToLightSource() const
{
	return _directionToLightSource;
}

void LightDirectional::SetDirectionToLightSource(const Vector3D& direction)
{
	_directionToLightSource = direction;
}

// Operator overloads
LightDirectional& LightDirectional::operator=(const LightDirectional& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Private methods
void LightDirectional::Copy(const LightDirectional& ld)
{
	_redIntensity = ld.GetRedIntensity();
	_greenIntensity = ld.GetGreenIntensity();
	_blueIntensity = ld.GetBlueIntensity();
	_directionToLightSource = ld.GetDirectionToLightSource();
}
