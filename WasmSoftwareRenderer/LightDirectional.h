#pragma once
#include "Light.h"
#include "Vector3D.h"

class LightDirectional : public Light
{
public:
	LightDirectional(void);
	LightDirectional(float redIntensity, float greenIntensity, float blueIntensity, Vector3D direction);
	LightDirectional(const LightDirectional& ld);
	~LightDirectional(void);

	Vector3D GetDirectionToLightSource() const;
	void SetDirectionToLightSource(const Vector3D& direction);

	LightDirectional& operator= (const LightDirectional& rhs);

private:
	Vector3D _directionToLightSource;

	void Copy(const LightDirectional& ld);
};
