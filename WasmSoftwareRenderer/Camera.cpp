#include "Camera.h"
#include <cmath>

// Constructors
Camera::Camera(void)
{
	// set sensible defaults for an empty constructor
	Initialise(0, 0, 0,
			   Vertex(0, 0, -50.0f, 1.0f),
			   640, 480);
}

Camera::Camera(float xRotation, float yRotation, float zRotation,
			   const Vertex& position,
			   unsigned int viewWidth, unsigned int viewHeight)
{
	Initialise(xRotation, yRotation, zRotation,
			   position,
			   viewWidth, viewHeight);
}

Camera::Camera(const Camera& cam)
{
	Copy(cam);
}

// Destructor
Camera::~Camera() {}

// Operator overloads
Camera& Camera::operator=(const Camera& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Properties
float Camera::GetXRotation(void) const
{
	return _xRotation;
}

float Camera::GetYRotation(void) const
{
	return _yRotation;
}

float Camera::GetZRotation(void) const
{
	return _zRotation;
}

Vertex Camera::GetPosition(void) const
{
	return _position;
}

void Camera::SetPosition(const Vertex& pos)
{
	_position = pos;
}

unsigned int Camera::GetViewWidth(void) const
{
	return _viewWidth;
}

unsigned int Camera::GetViewHeight(void) const
{
	return _viewHeight;
}

Matrix3D Camera::GetViewpointTransformation(void) const
{
	return _viewpointTrans;
}

Matrix3D Camera::GetPerspectiveTransformation(void) const
{
	return _perspectiveTrans;
}

Matrix3D Camera::GetScreenTransformation(void) const
{
	return _screenTrans;
}

// Private methods
void Camera::Initialise(float xRotation, float yRotation, float zRotation,
						const Vertex& position,
						unsigned int viewWidth, unsigned int viewHeight)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_position = position;
	_viewWidth = viewWidth;
	_viewHeight = viewHeight;

	// Camera rotations, defined for all 3 axis
	// and multiplied together for the final camera matrix
	Matrix3D cameraX(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, cos(_xRotation), sin(_xRotation), 0.0f,
					 0.0f, sin(_xRotation) * -1.0f, cos(_xRotation), 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	Matrix3D cameraY(cos(_yRotation), 0.0f, sin(_yRotation) * -1.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 sin(_yRotation), 0.0f, cos(_yRotation), 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	Matrix3D cameraZ(cos(_zRotation), sin(_zRotation), 0.0f, 0.0f,
					 sin(_zRotation) * -1.0f, cos(_zRotation), 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	Matrix3D cameraW(1.0f, 0.0f, 0.0f, _position.GetX() * -1.0f,
					 0.0f, 1.0f, 0.0f, _position.GetY() * -1.0f,
					 0.0f, 0.0f, 1.0f, _position.GetZ() * -1.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	_viewpointTrans = cameraX * cameraY * cameraZ * cameraW;

	// Perspective Transformation
	_perspectiveTrans = Matrix3D(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f);

	// Screen Transformation
	_screenTrans = Matrix3D(_viewHeight / 2.0f, 0.0f, 0.0f, _viewWidth / 2.0f,
							0.0f, (_viewHeight / 2.0f) * -1.0f, 0.0f, _viewHeight / 2.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
}

void Camera::Copy(const Camera& cam)
{
	_xRotation = cam.GetXRotation();
	_yRotation = cam.GetYRotation();
	_zRotation = cam.GetZRotation();
	_position = cam.GetPosition();
	_viewWidth = cam.GetViewWidth();
	_viewHeight = cam.GetViewHeight();
	_viewpointTrans = cam.GetViewpointTransformation();
	_perspectiveTrans = cam.GetPerspectiveTransformation();
	_screenTrans = cam.GetScreenTransformation();
}
