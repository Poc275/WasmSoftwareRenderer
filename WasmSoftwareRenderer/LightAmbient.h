#pragma once
#include "Light.h"

class LightAmbient : public Light
{
public:
	LightAmbient(void);
	LightAmbient(float redIntensity, float greenIntensity, float blueIntensity,
				 float ambientReflectionRed, float amnbientReflectionGreen, float ambientReflectionBlue);
	LightAmbient(const LightAmbient& la);
	~LightAmbient(void);

	float GetAmbientReflectionRed() const;
	void SetAmbientReflectionRed(float reflectionRed);
	float GetAmbientReflectionGreen() const;
	void SetAmbientReflectionGreen(float reflectionGreen);
	float GetAmbientReflectionBlue() const;
	void SetAmbientReflectionBlue(float reflectionBlue);

	LightAmbient& operator= (const LightAmbient& rhs);

private:
	float _ambientReflectionRed;
	float _ambientReflectionGreen;
	float _ambientReflectionBlue;

	void Copy(const LightAmbient& la);
};
