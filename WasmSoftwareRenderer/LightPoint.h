#pragma once
#include "Light.h"
#include "Vertex.h"

class LightPoint : public Light
{
public:
	LightPoint(void);
	LightPoint(float redIntensity, float greenIntensity, float blueIntensity, Vertex position,
		float attenuationA, float attenuationB, float attenuationC, bool switchState);
	LightPoint(const LightPoint& lp);
	~LightPoint(void);

	Vertex GetLightPosition() const;
	void SetLightPosition(const Vertex& pos);
	float GetAttenuationA() const;
	void SetAttenuationA(const float attA);
	float GetAttenuationB() const;
	void SetAttenuationB(const float attB);
	float GetAttenuationC() const;
	void SetAttenuationC(const float attC);
	bool GetLightSwitch(void) const;
	void SetLightSwitch(bool switchState);

	LightPoint& operator= (const LightPoint& rhs);

private:
	Vertex _position;
	float _attenuationA;
	float _attenuationB;
	float _attenuationC;
	bool _switch;

	void Copy(const LightPoint& lp);
};
