#pragma once
#include "Coordinate.h"

class Vector3D : public Coordinate
{
public:
	Vector3D(void);
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& v);
	~Vector3D(void);

	Vector3D& operator= (const Vector3D& rhs);
	float operator* (const Vector3D& other);
	Vector3D operator+ (const Vector3D& rhs);
	Vector3D operator/ (const float rhs);
	Vector3D operator- (const Vector3D& rhs);

	Vector3D CrossProduct(const Vector3D& other);
	Vector3D& Normalise();
	float GetVectorLength() const;
	Vector3D& ScaleVector(float scalar);

private:
	void Copy(const Vector3D& v);
};
