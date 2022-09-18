#include "Light.h"

// Constructors
Light::Light(void)
{
	// set sensible defaults for default constructor
	_redIntensity = 100.0f;
	_greenIntensity = 100.0f;
	_blueIntensity = 100.0f;
}

Light::Light(float redIntensity, float greenIntensity, float blueIntensity)
{
	_redIntensity = redIntensity;
	_greenIntensity = greenIntensity;
	_blueIntensity = blueIntensity;
}

Light::Light(const Light& l)
{
	Copy(l);
}

// Destructor
Light::~Light(void) {}

// Properties
float Light::GetRedIntensity() const
{
	return _redIntensity;
}

float Light::GetGreenIntensity() const
{
	return _greenIntensity;
}

float Light::GetBlueIntensity() const
{
	return _blueIntensity;
}

void Light::SetRedIntensity(float red)
{
	_redIntensity = red;
}

void Light::SetGreenIntensity(float green)
{
	_greenIntensity = green;
}

void Light::SetBlueIntensity(float blue)
{
	_blueIntensity = blue;
}

// Operator overloads
Light& Light::operator=(const Light& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

void Light::Copy(const Light& l)
{
	_redIntensity = l.GetRedIntensity();
	_greenIntensity = l.GetGreenIntensity();
	_blueIntensity = l.GetBlueIntensity();
}
