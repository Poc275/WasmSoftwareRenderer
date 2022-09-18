#pragma once
#include "Coordinate.h"
#include "Vector3D.h"

class Vertex : public Coordinate
{
public:
	Vertex(void);
	Vertex(float x, float y, float z, float w);
	Vertex(const Vertex& v);
	~Vertex(void);

	Vertex& operator= (const Vertex& rhs);
	Vector3D operator- (const Vertex& rhs);
	Vertex operator+ (const Vector3D& rhs);

	Vertex& DehomogenizeVertex();

private:
	void Copy(const Vertex& v);
};
