#pragma once
#include "Vertex.h"
#include "Polygon3D.h"
#include "Matrix3D.h"
#include <vector>

using namespace std;

class Model3D
{
public:
	Model3D(void);
	Model3D(const Model3D& m);
	~Model3D();

	Model3D& operator= (const Model3D& rhs);

	Vertex GetVertex(int index) const;
	Vertex GetTransformedVertex(int index) const;
	Polygon3D GetPolygon(int index) const;
	int GetPolygons() const;

	void AddPolygon(Polygon3D poly);
	void AddVertex(Vertex vert);
	void ApplyTransformToLocalVertices(const Matrix3D& transform);
	void ApplyTransformToTransformedVertices(const Matrix3D& transform);
	void Dehomogenize(void);

private:
	vector<Vertex> _vertices;
	vector<Polygon3D> _polygons;
	vector<Vertex> _transVerts;

	void Copy(const Model3D& m);
};
