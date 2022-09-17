#pragma once
#include "Coordinate.h"

class Vertex : public Coordinate
{
public:
	Vertex(void);
	Vertex(float x, float y, float z, float w);
	Vertex(const Vertex& v);
	~Vertex(void);

	Vertex& operator= (const Vertex& rhs);

	Vertex& DehomogenizeVertex();

private:
	void Copy(const Vertex& v);
};
