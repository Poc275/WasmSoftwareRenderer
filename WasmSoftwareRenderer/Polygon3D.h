#pragma once
#include "Vector3D.h"

class Polygon3D
{
public:
	Polygon3D(void);
	Polygon3D(int indexOne, int indexTwo, int indexThree);
	Polygon3D(const Polygon3D& p);
	~Polygon3D();

	Polygon3D& operator= (const Polygon3D& rhs);

	int GetVertexIndex(int index) const;
	void SetBackFacing(bool backwards);
	bool DrawPolygon(void) const;

	Vector3D GetPolygonNormal(void) const;
	void SetPolygonNormal(const Vector3D& normal);

private:
	int _vertIndex[3];
	bool _isBackFacing;

	Vector3D _normal;

	void Copy(const Polygon3D& p);
};
