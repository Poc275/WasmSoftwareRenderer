#pragma once
#include "Matrix3D.h"
#include "Vertex.h"

class Camera
{
public:
	Camera(void);
	Camera(float xRotation, float yRotation, float zRotation,
		   const Vertex& position,
		   unsigned int viewWidth, unsigned int viewHeight);
	Camera(const Camera& cam);
	~Camera();

	Camera& operator= (const Camera& rhs);

	float GetXRotation(void) const;
	float GetYRotation(void) const;
	float GetZRotation(void) const;
	Vertex GetPosition(void) const;
	void SetPosition(const Vertex& pos);
	unsigned int GetViewWidth(void) const;
	unsigned int GetViewHeight(void) const;
	Matrix3D GetViewpointTransformation(void) const;
	Matrix3D GetPerspectiveTransformation(void) const;
	Matrix3D GetScreenTransformation(void) const;

private:
	float _xRotation;
	float _yRotation;
	float _zRotation;
	Vertex _position;
	unsigned int _viewWidth;
	unsigned int _viewHeight;

	Matrix3D _viewpointTrans;
	Matrix3D _perspectiveTrans;
	Matrix3D _screenTrans;

	void Initialise(float xRotation, float yRotation, float zRotation,
					const Vertex& position,
					unsigned int viewWidth, unsigned int viewHeight);
	void Copy(const Camera& cam);
};
