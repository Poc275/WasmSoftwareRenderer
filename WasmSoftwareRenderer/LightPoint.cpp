#include "LightPoint.h"

// Constructors
LightPoint::LightPoint(void) : Light()
{
	_position = Vertex();
	_attenuationA = 0.0f;
	_attenuationB = 0.0f;
	_attenuationC = 0.0f;
	_switch = true;
}

LightPoint::LightPoint(float redIntensity, float greenIntensity, float blueIntensity,
					   Vertex position,
					   float attenuationA, float attenuationB, float attenuationC,
					   bool switchState)
	: Light(redIntensity, greenIntensity, blueIntensity)
{
	_position = position;
	_attenuationA = attenuationA;
	_attenuationB = attenuationB;
	_attenuationC = attenuationC;
	_switch = switchState;
}

LightPoint::LightPoint(const LightPoint& lp)
{
	Copy(lp);
}

// Destructor
LightPoint::~LightPoint(void) {}

// Properties
Vertex LightPoint::GetLightPosition() const
{
	return _position;
}

void LightPoint::SetLightPosition(const Vertex& pos)
{
	_position = pos;
}

float LightPoint::GetAttenuationA() const
{
	return _attenuationA;
}

void LightPoint::SetAttenuationA(const float attA)
{
	_attenuationA = attA;
}

float LightPoint::GetAttenuationB() const
{
	return _attenuationB;
}

void LightPoint::SetAttenuationB(const float attB)
{
	_attenuationB = attB;
}

float LightPoint::GetAttenuationC() const
{
	return _attenuationC;
}

void LightPoint::SetAttenuationC(const float attC)
{
	_attenuationC = attC;
}

bool LightPoint::GetLightSwitch(void) const
{
	return _switch;
}

void LightPoint::SetLightSwitch(bool switchState)
{
	_switch = switchState;
}

// Operator overloads
LightPoint& LightPoint::operator=(const LightPoint& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Private methods
void LightPoint::Copy(const LightPoint& lp)
{
	_redIntensity = lp.GetRedIntensity();
	_greenIntensity = lp.GetGreenIntensity();
	_blueIntensity = lp.GetBlueIntensity();
	_position = lp.GetLightPosition();
	_attenuationA = lp.GetAttenuationA();
	_attenuationB = lp.GetAttenuationB();
	_attenuationC = lp.GetAttenuationC();
	_switch = lp.GetLightSwitch();
}
