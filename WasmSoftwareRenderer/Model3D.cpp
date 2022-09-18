#include "Model3D.h"

// Constructors
Model3D::Model3D(void)
{
	_worldRotations = Vector3D(0.0f, 0.0f, 0.0f);
	_worldTranslations = Vector3D(0.0f, 0.0f, 0.0f);
	_worldScales = Vector3D(1.0f, 1.0f, 1.0f);
}

Model3D::Model3D(const Model3D& m)
{
	Copy(m);
}

// Destructor
Model3D::~Model3D() {}

// Operator overloads
Model3D& Model3D::operator=(const Model3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Properties
Vertex Model3D::GetVertex(int index) const
{
	return _vertices.at(index);
}

Vertex Model3D::GetTransformedVertex(int index) const
{
	return _transVerts.at(index);
}

Polygon3D Model3D::GetPolygon(int index) const
{
	return _polygons.at(index);
}

int Model3D::GetPolygons() const
{
	return _polygons.size();
}

Vector3D Model3D::GetWorldRotations(void) const
{
	return _worldRotations;
}

Vector3D Model3D::GetWorldTranslations(void) const
{
	return _worldTranslations;
}

Vector3D Model3D::GetWorldScales(void) const
{
	return _worldScales;
}

void Model3D::SetWorldRotations(const Vector3D& worldRot)
{
	_worldRotations = worldRot;
}

void Model3D::SetWorldTranslations(const Vector3D& worldTrans)
{
	_worldTranslations = worldTrans;
}

void Model3D::SetWorldScales(const Vector3D& worldScales)
{
	_worldScales = worldScales;
}

// Public methods
void Model3D::AddPolygon(Polygon3D poly)
{
	_polygons.push_back(poly);
}

void Model3D::AddVertex(Vertex vert)
{
	_vertices.push_back(vert);
}

void Model3D::ApplyTransformToLocalVertices(const Matrix3D& transform)
{
	// Multiply the transformation matrix to each vertex in the
	// original collection and store it in _transVerts, but first
	// clear the list otherwise the collection will keep increasing
	_transVerts.clear();

	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		Vertex v = transform * _vertices.at(i);
		_transVerts.push_back(v);
	}
}

void Model3D::ApplyTransformToTransformedVertices(const Matrix3D& transform)
{
	// Multiply the 2nd transformation matrix to each vertex in the
	// _transVerts collection and overwrite
	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		// Apply transformation
		_transVerts.at(i) = transform * _transVerts.at(i);
	}
}

void Model3D::Dehomogenize(void)
{
	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		// Store w value in pre transform Z
		// for UV projection correction
		//_transVerts.at(i).SetPreTransformZ(_transVerts.at(i).GetW());

		// We only need to dehomogenise if w is not equal to 1
		// as result will be the same as the original verts if w is 1
		if (_transVerts.at(i).GetW() != 1.0f)
		{
			_transVerts.at(i).DehomogenizeVertex();
		}
	}
}

// Private methods
void Model3D::Copy(const Model3D& m)
{
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		_vertices.at(i) = m.GetVertex(i);
	}

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		_polygons.at(i) = m.GetPolygon(i);
	}
}
