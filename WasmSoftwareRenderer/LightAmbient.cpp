#include "LightAmbient.h"

// Constructors
LightAmbient::LightAmbient(void) : Light()
{
	_ambientReflectionRed = 1.0f;
	_ambientReflectionGreen = 1.0f;
	_ambientReflectionBlue = 1.0f;
}

LightAmbient::LightAmbient(float redIntensity, float greenIntensity, float blueIntensity,
						   float ambientReflectionRed, float amnbientReflectionGreen, float ambientReflectionBlue)
	: Light(redIntensity, greenIntensity, blueIntensity)
{
	_ambientReflectionRed = ambientReflectionRed;
	_ambientReflectionGreen = amnbientReflectionGreen;
	_ambientReflectionBlue = ambientReflectionBlue;
}

LightAmbient::LightAmbient(const LightAmbient& la)
{
	Copy(la);
}

// Destructor
LightAmbient::~LightAmbient(void) {}

// Properties
float LightAmbient::GetAmbientReflectionRed() const
{
	return _ambientReflectionRed;
}

void LightAmbient::SetAmbientReflectionRed(float reflectionRed)
{
	_ambientReflectionRed = reflectionRed;
}

float LightAmbient::GetAmbientReflectionGreen() const
{
	return _ambientReflectionGreen;
}

void LightAmbient::SetAmbientReflectionGreen(float reflectionGreen)
{
	_ambientReflectionGreen = reflectionGreen;
}

float LightAmbient::GetAmbientReflectionBlue() const
{
	return _ambientReflectionBlue;
}

void LightAmbient::SetAmbientReflectionBlue(float reflectionBlue)
{
	_ambientReflectionBlue = reflectionBlue;
}

// Operator overloads
LightAmbient& LightAmbient::operator=(const LightAmbient& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Private methods
void LightAmbient::Copy(const LightAmbient& la)
{
	_redIntensity = la.GetRedIntensity();
	_greenIntensity = la.GetGreenIntensity();
	_blueIntensity = la.GetBlueIntensity();
	_ambientReflectionRed = la.GetAmbientReflectionRed();
	_ambientReflectionGreen = la.GetAmbientReflectionGreen();
	_ambientReflectionBlue = la.GetAmbientReflectionBlue();
}
