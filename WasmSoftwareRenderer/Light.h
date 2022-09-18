#pragma once

// Base class for a light object
class Light
{
public:
	Light(void);
	Light(float redIntensity, float greenIntensity, float blueIntensity);
	Light(const Light& l);
	~Light(void);

	float GetRedIntensity() const;
	float GetGreenIntensity() const;
	float GetBlueIntensity() const;
	void SetRedIntensity(float red);
	void SetGreenIntensity(float green);
	void SetBlueIntensity(float blue);

	Light& operator= (const Light& rhs);

protected:
	float _redIntensity;
	float _greenIntensity;
	float _blueIntensity;

private:
	void Copy(const Light& l);
};
