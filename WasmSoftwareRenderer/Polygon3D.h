#pragma once
#include <SDL.h>
#include "Vector3D.h"

class Polygon3D
{
public:
	Polygon3D(void);
	Polygon3D(int indexOne, int indexTwo, int indexThree);
	Polygon3D(int indexOne, int indexTwo, int indexThree, int textureOne, int textureTwo, int textureThree);
	Polygon3D(const Polygon3D& p);
	~Polygon3D();

	Polygon3D& operator= (const Polygon3D& rhs);

	int GetVertexIndex(int index) const;
	void SetBackFacing(bool backwards);
	bool DrawPolygon(void) const;
	float GetAverageZDepth(void) const;
	void SetAverageZDepth(float averageZ);
	SDL_Color GetLightingColour(void) const;
	void SetLightingColour(const SDL_Color& color);
	Vector3D GetPolygonNormal(void) const;
	void SetPolygonNormal(const Vector3D& normal);
	int GetTextureIndex(int index) const;

private:
	int _vertIndex[3];
	bool _isBackFacing;
	float _averageZDepth;
	SDL_Color _lightingColour;
	Vector3D _normal;
	int _textureIndex[3];

	void Copy(const Polygon3D& p);
};
