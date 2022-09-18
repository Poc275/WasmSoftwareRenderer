#include "Model3D.h"
#include <algorithm>

// Constructors
Model3D::Model3D(void)
{
	_diffuseRed = 0.5f;
	_diffuseGreen = 0.5f;
	_diffuseBlue = 0.5f;
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

float Model3D::GetDiffuseRed() const
{
	return _diffuseRed;
}

void Model3D::SetDiffuseRed(float diffuseR)
{
	_diffuseRed = diffuseR;
}

float Model3D::GetDiffuseGreen() const
{
	return _diffuseGreen;
}

void Model3D::SetDiffuseGreen(float diffuseG)
{
	_diffuseGreen = diffuseG;
}

float Model3D::GetDiffuseBlue() const
{
	return _diffuseBlue;
}

void Model3D::SetDiffuseBlue(float diffuseB)
{
	_diffuseBlue = diffuseB;
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

void Model3D::CalculateBackfaces(const Vertex& cameraPosition)
{
	Vertex verts[3];

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Reset backfacing polygons
		if (!_polygons.at(i).DrawPolygon())
		{
			_polygons.at(i).SetBackFacing(false);
		}

		// Get polygon indexes and vertices from them and 
		// add to collection ready for normal calculation
		for (unsigned int j = 0; j < 3; j++)
		{
			int polyIndex = _polygons.at(i).GetVertexIndex(j);
			Vertex vert = _transVerts.at(polyIndex);
			verts[j] = vert;
		}
		// Construct 2 vectors for this polygon
		Vector3D vectorA = verts[1] - verts[0];
		Vector3D vectorB = verts[2] - verts[0];

		// Calculate cross product and normalise to get perpendicular vector
		Vector3D normal = vectorA.CrossProduct(vectorB);
		normal.Normalise();

		// Store normalised polygon vector inside polygon ready for lighting calculations
		_polygons.at(i).SetPolygonNormal(normal);

		// Get eye (camera) direction and compare to normal vector
		// by calculating the dot product
		Vector3D eyeVector = verts[0] - cameraPosition;
		float dotProduct = normal * eyeVector;

		// Set polygon to be backfacing if dotProduct is less than zero
		if (dotProduct < 0)
		{
			_polygons.at(i).SetBackFacing(true);
		}
	}
}

void Model3D::Sort(void)
{
	float averageZDepth = 0.0f;

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int polyIndex = _polygons.at(i).GetVertexIndex(j);
			Vertex vert = _transVerts.at(polyIndex);
			averageZDepth += vert.GetZ();
		}
		// Store average z depth inside polygon and reset
		// the average Z depth counter ready for the next polygon
		_polygons.at(i).SetAverageZDepth(averageZDepth / 3);
		averageZDepth = 0.0f;
	}
	// Now sort the polygon collection according to z depth
	std::sort(_polygons.begin(), _polygons.end(), &ZDepthSorter);
}

// Lighting methods
void Model3D::CalculateLightingAmbient(const LightAmbient& ambientLight)
{
	float totalRed = 0.0f;
	float totalGreen = 0.0f;
	float totalBlue = 0.0f;
	float tempRed = 0.0f;
	float tempGreen = 0.0f;
	float tempBlue = 0.0f;
	SDL_Color ambientLightColour;

	// Reset totals to black
	totalRed = 0.0f;
	totalGreen = 0.0f;
	totalBlue = 0.0f;

	tempRed = ambientLight.GetRedIntensity();
	tempGreen = ambientLight.GetGreenIntensity();
	tempBlue = ambientLight.GetBlueIntensity();

	tempRed *= ambientLight.GetAmbientReflectionRed();
	tempGreen *= ambientLight.GetAmbientReflectionGreen();
	tempBlue *= ambientLight.GetAmbientReflectionBlue();

	// Add temp RGB to total RGB
	totalRed += tempRed;
	totalGreen += tempGreen;
	totalBlue += tempBlue;

	// Clamp total RGB values in range 0-255
	if (totalRed < 0 || totalRed > 255)
	{
		if (totalRed < 0)
		{
			totalRed = 0.0f;
		}
		else
		{
			totalRed = 255.0f;
		}
	}

	if (totalGreen < 0 || totalGreen > 255)
	{
		if (totalGreen < 0)
		{
			totalGreen = 0.0f;
		}
		else
		{
			totalGreen = 255.0f;
		}
	}

	if (totalBlue < 0 || totalBlue > 255)
	{
		if (totalBlue < 0)
		{
			totalBlue = 0.0f;
		}
		else
		{
			totalBlue = 255.0f;
		}
	}

	// Build RGBA colour using the total RGB values
	ambientLightColour = SDL_Color{ static_cast<unsigned char>(totalRed),
									static_cast<unsigned char>(totalGreen),
									static_cast<unsigned char>(totalBlue),
									255 };

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Store colour in polygon ready to paint
		_polygons.at(i).SetLightingColour(ambientLightColour);
	}

	// Also store ambient light colour for all vertices for Gouraud shading
	/*for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		_transVerts.at(i).SetVertexColour(ambientLightColour);
	}*/
}

void Model3D::CalculateLightingDirectional(const vector<LightDirectional>& directionalLights)
{
	float totalRed = 0.0f;
	float totalGreen = 0.0f;
	float totalBlue = 0.0f;
	float tempRed = 0.0f;
	float tempGreen = 0.0f;
	float tempBlue = 0.0f;

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Initialise total colours to results from ambient lighting calculation
		SDL_Color ambientLightColour = _polygons.at(i).GetLightingColour();

		totalRed = ambientLightColour.r;
		totalGreen = ambientLightColour.g;
		totalBlue = ambientLightColour.b;

		for (unsigned int j = 0; j < directionalLights.size(); j++)
		{
			// Set temp RGB values to light RGB intensity
			tempRed = directionalLights.at(j).GetRedIntensity();
			tempGreen = directionalLights.at(j).GetGreenIntensity();
			tempBlue = directionalLights.at(j).GetBlueIntensity();

			// Multiply temp RGB by the corresponding model diffuse reflection co-efficients
			tempRed *= this->GetDiffuseRed();
			tempGreen *= this->GetDiffuseGreen();
			tempBlue *= this->GetDiffuseBlue();

			// Calculate dot product of light position vector and polygon normal vector
			float lightDotProduct = directionalLights.at(j).GetDirectionToLightSource() *
				_polygons.at(i).GetPolygonNormal();

			// Only add light effects if dot product is > 0
			if (lightDotProduct > 0)
			{
				// Multiply temp RGB by dot product
				tempRed *= lightDotProduct;
				tempGreen *= lightDotProduct;
				tempBlue *= lightDotProduct;

				// Add temp RGB to total RGB
				totalRed += tempRed;
				totalGreen += tempGreen;
				totalBlue += tempBlue;
			}
		}

		// Clamp total RGB values in range 0-255
		if (totalRed < 0 || totalRed > 255)
		{
			if (totalRed < 0)
			{
				totalRed = 0.0f;
			}
			else
			{
				totalRed = 255.0f;
			}
		}

		if (totalGreen < 0 || totalGreen > 255)
		{
			if (totalGreen < 0)
			{
				totalGreen = 0.0f;
			}
			else
			{
				totalGreen = 255.0f;
			}
		}

		if (totalBlue < 0 || totalBlue > 255)
		{
			if (totalBlue < 0)
			{
				totalBlue = 0.0f;
			}
			else
			{
				totalBlue = 255.0f;
			}
		}

		// Store colour in polygon ready to paint
		_polygons.at(i).SetLightingColour(SDL_Color{ static_cast<unsigned char>(totalRed),
													 static_cast<unsigned char>(totalGreen),
													 static_cast<unsigned char>(totalBlue),
													 255 });
	}
}

void Model3D::CalculateLightingPoint(const vector<LightPoint>& pointLights)
{
	float totalRed = 0.0f;
	float totalGreen = 0.0f;
	float totalBlue = 0.0f;
	float tempRed = 0.0f;
	float tempGreen = 0.0f;
	float tempBlue = 0.0f;

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Initialise total colours to results from directional lighting calculations
		SDL_Color directionalLightColour = _polygons.at(i).GetLightingColour();

		totalRed = directionalLightColour.r;
		totalGreen = directionalLightColour.g;
		totalBlue = directionalLightColour.b;

		for (unsigned int j = 0; j < pointLights.size(); j++)
		{
			if (pointLights.at(j).GetLightSwitch())
			{
				// Set temp RGB values to point light RGB intensity
				tempRed = pointLights.at(j).GetRedIntensity();
				tempGreen = pointLights.at(j).GetGreenIntensity();
				tempBlue = pointLights.at(j).GetBlueIntensity();

				// Multiply temp RGBs by the corresponding model diffuse reflection co-efficients
				tempRed *= this->GetDiffuseRed();
				tempGreen *= this->GetDiffuseGreen();
				tempBlue *= this->GetDiffuseBlue();

				// Calculate vector to light source
				int polygonFirstVertexIndex = _polygons.at(i).GetVertexIndex(0);
				Vertex polygonFirstVertex = _transVerts.at(polygonFirstVertexIndex);
				Vector3D pointLightSource = polygonFirstVertex - pointLights.at(j).GetLightPosition();

				// Get vector length to light source before we normalise it
				// as we need the length for the attenuation coefficient calculation
				float distanceFromLightSource = pointLightSource.GetVectorLength();

				// Now we can normalise the light source vector
				pointLightSource.Normalise();

				// Now we have the point light source vector, we can calculate
				// the dot product against the polygon normal
				float lightDotProduct = pointLightSource * _polygons.at(i).GetPolygonNormal();

				// Only add light effects if dot product is > 0
				if (lightDotProduct > 0)
				{
					// Multiply temp RGBs by dot product
					tempRed *= lightDotProduct;
					tempGreen *= lightDotProduct;
					tempBlue *= lightDotProduct;

					// Attenuation Calculation
					float attenuation = pointLights.at(j).GetAttenuationA() + pointLights.at(j).GetAttenuationB() *
						distanceFromLightSource + pointLights.at(j).GetAttenuationC() * distanceFromLightSource * distanceFromLightSource;
					float inverseAttenuation = 1.0f / attenuation;

					// Attenuation fudge factor to boost value
					inverseAttenuation *= 15.0f;

					// Multiply temp RGBs by attenuation result
					tempRed *= inverseAttenuation;
					tempGreen *= inverseAttenuation;
					tempBlue *= inverseAttenuation;

					// Add temp RGBs to total RGBs
					totalRed += tempRed;
					totalGreen += tempGreen;
					totalBlue += tempBlue;
				}
			}
		}

		// Clamp total RGB values in range 0-255
		if (totalRed < 0 || totalRed > 255)
		{
			if (totalRed < 0)
			{
				totalRed = 0.0f;
			}
			else
			{
				totalRed = 255.0f;
			}
		}

		if (totalGreen < 0 || totalGreen > 255)
		{
			if (totalGreen < 0)
			{
				totalGreen = 0.0f;
			}
			else
			{
				totalGreen = 255.0f;
			}
		}

		if (totalBlue < 0 || totalBlue > 255)
		{
			if (totalBlue < 0)
			{
				totalBlue = 0.0f;
			}
			else
			{
				totalBlue = 255.0f;
			}
		}

		// Store colour in polygon ready to paint
		_polygons.at(i).SetLightingColour(SDL_Color{ static_cast<unsigned char>(totalRed),
													 static_cast<unsigned char>(totalGreen),
													 static_cast<unsigned char>(totalBlue),
													 255 });
	}
}

// Private methods
bool Model3D::ZDepthSorter(const Polygon3D& poly1, const Polygon3D& poly2)
{
	return poly1.GetAverageZDepth() > poly2.GetAverageZDepth();
}

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

	_diffuseRed = m.GetDiffuseRed();
	_diffuseGreen = m.GetDiffuseGreen();
	_diffuseBlue = m.GetDiffuseBlue();

	_worldRotations = m.GetWorldRotations();
	_worldTranslations = m.GetWorldTranslations();
	_worldScales = m.GetWorldScales();
}
