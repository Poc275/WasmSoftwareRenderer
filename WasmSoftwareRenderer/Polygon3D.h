#pragma once
class Polygon3D
{
public:
	Polygon3D(void);
	Polygon3D(int indexOne, int indexTwo, int indexThree);
	Polygon3D(const Polygon3D& p);
	~Polygon3D();

	Polygon3D& operator= (const Polygon3D& rhs);

	int GetVertexIndex(int index) const;

private:
	int _vertIndex[3];

	void Copy(const Polygon3D& p);
};
