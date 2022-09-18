#pragma once
#include <SDL.h>
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

	Vector3D GetVertexNormal() const;
	void SetVertexNormal(const Vector3D& normal);
	SDL_Color GetVertexColour() const;
	void SetVertexColour(const SDL_Color& colour);
	int GetNormalCount() const;
	void SetNormalCount(int count);
	void IncrementNormalCount();

	Vertex& DehomogenizeVertex();

private:
	Vector3D _normal;
	SDL_Color _colour;
	int _count;

	void Copy(const Vertex& v);
};
