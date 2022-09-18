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

	Vector3D GetWorldRotations(void) const;
	Vector3D GetWorldTranslations(void) const;
	Vector3D GetWorldScales(void) const;
	void SetWorldRotations(const Vector3D& worldRot);
	void SetWorldTranslations(const Vector3D& worldTrans);
	void SetWorldScales(const Vector3D& worldScales);

	void AddPolygon(Polygon3D poly);
	void AddVertex(Vertex vert);
	void ApplyTransformToLocalVertices(const Matrix3D& transform);
	void ApplyTransformToTransformedVertices(const Matrix3D& transform);
	void Dehomogenize(void);

private:
	vector<Vertex> _vertices;
	vector<Polygon3D> _polygons;
	vector<Vertex> _transVerts;

	Vector3D _worldRotations;
	Vector3D _worldTranslations;
	Vector3D _worldScales;

	void Copy(const Model3D& m);
};