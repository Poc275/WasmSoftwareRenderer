#include "Model3D.h"
#include <algorithm>

typedef unsigned char BYTE;

// Constructors
Model3D::Model3D(void)
{
	_diffuseRed = 0.5f;
	_diffuseGreen = 0.5f;
	_diffuseBlue = 0.5f;

	_texture = new BYTE();
	_palette = new SDL_Color();
	_textureWidth = 0;

	_worldRotations = Vector3D(0.0f, 0.0f, 0.0f);
	_worldTranslations = Vector3D(0.0f, 0.0f, 0.0f);
	_worldScales = Vector3D(1.0f, 1.0f, 1.0f);
}

Model3D::Model3D(const Model3D& m)
{
	Copy(m);
}

// Destructor
Model3D::~Model3D()
{
	if (this->GetTextureMapWidth() <= 0)
	{
		// No texture map was provided so
		// we have nothing to delete
	}
	else
	{
		delete[] _texture;
		delete[] _palette;
	}
}

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

Texture Model3D::GetTexture(int index) const
{
	return _UVs.at(index);
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

BYTE* Model3D::GetTextureMap() const
{
	return _texture;
}

void Model3D::SetTextureMap(BYTE* texturemap)
{
	_texture = texturemap;
}

SDL_Color* Model3D::GetPalette() const
{
	return _palette;
}

void Model3D::SetPalette(SDL_Color* palette)
{
	_palette = palette;
}

void Model3D::SetTextureMapWidth(const int width)
{
	_textureWidth = width;
}

int Model3D::GetTextureMapWidth() const
{
	return _textureWidth;
}

SDL_Color Model3D::GetPaletteColour(int index) const
{
	int paletteIndex = _texture[index];
	return _palette[paletteIndex];
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

void Model3D::AddTexture(Texture texture)
{
	_UVs.push_back(texture);
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
		// Get a copy of vertex member variables that we do
		// not want to overwrite
		SDL_Color vertColour = _transVerts.at(i).GetVertexColour();
		Vector3D vertNormal = _transVerts.at(i).GetVertexNormal();
		//float fogDensity = _transVerts.at(i).GetFogDensity();
		//float preTransZ = _transVerts.at(i).GetPreTransformZ();

		// Apply transformation
		_transVerts.at(i) = transform * _transVerts.at(i);

		// Re-apply the member variables once the transform
		// has taken place
		_transVerts.at(i).SetVertexColour(vertColour);
		_transVerts.at(i).SetVertexNormal(vertNormal);
		//_transVerts.at(i).SetFogDensity(fogDensity);
		//_transVerts.at(i).SetPreTransformZ(preTransZ);
	}

	// Also transform shadow verts
	//for (unsigned int i = 0; i < _polygons.size(); i++)
	//{
	//	for (unsigned int j = 0; j < 3; j++)
	//	{
	//		Vertex shadowVert = _polygons.at(i).GetPolygonShadowVertex(j);
	//		shadowVert = transform * shadowVert;
	//		_polygons.at(i).SetPolygonShadowVertex(shadowVert, j);
	//	}
	//}
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

	// Also de-homogenize shadow verts
	//for (unsigned int i = 0; i < _polygons.size(); i++)
	//{
	//	for (unsigned int j = 0; j < 3; j++)
	//	{
	//		Vertex shadowVert = _polygons.at(i).GetPolygonShadowVertex(j);
	//		// We only need to dehomogenise if w is not equal to 1
	//		// as result will be the same as the original verts if w is 1
	//		if (shadowVert.GetW() != 1.0f)
	//		{
	//			shadowVert.DehomogenizeVertex();
	//		}
	//		_polygons.at(i).SetPolygonShadowVertex(shadowVert, j);
	//	}
	//}
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

void Model3D::CalculateVertexNormals(void)
{
	Vertex verts[3];
	int polygonIndex = 0;

	// Reset each vertex normal and set count to zero
	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		_transVerts.at(i).SetVertexNormal(Vector3D(0.0f, 0.0f, 0.0f));
		_transVerts.at(i).SetNormalCount(0);
	}

	// Calculate polygon normals in same way as previously
	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Get polygon indexes and vertices from them and 
		// add to collection ready for normal calculation
		for (unsigned int j = 0; j < 3; j++)
		{
			polygonIndex = _polygons.at(i).GetVertexIndex(j);
			Vertex vert = _transVerts.at(polygonIndex);
			verts[j] = vert;
		}
		// Construct 2 vectors for this polygon
		Vector3D vectorA = verts[1] - verts[0];
		Vector3D vectorB = verts[2] - verts[0];

		// Calculate cross product and normalise to get perpendicular vector
		Vector3D normal = vectorA.CrossProduct(vectorB);

		// Add polygon normal to vertex normal for each of the 3 contributing vertices
		for (unsigned int j = 0; j < 3; j++)
		{
			polygonIndex = _polygons.at(i).GetVertexIndex(j);
			Vector3D currentVertexNormal = _transVerts.at(polygonIndex).GetVertexNormal();
			_transVerts.at(polygonIndex).SetVertexNormal(currentVertexNormal + normal);
			_transVerts.at(polygonIndex).IncrementNormalCount();
		}
	}

	// For each vertex, divide the summed vertex normal by the contributing count
	// and normalise the result
	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		Vector3D averagedNormal = _transVerts.at(i).GetVertexNormal() /
			(float)_transVerts.at(i).GetNormalCount();
		_transVerts.at(i).SetVertexNormal(averagedNormal.Normalise());
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
	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		_transVerts.at(i).SetVertexColour(ambientLightColour);
	}
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

void Model3D::CalculateLightingDirectionalGouraud(const vector<LightDirectional>& directionalLights)
{
	float totalRed = 0.0f;
	float totalGreen = 0.0f;
	float totalBlue = 0.0f;
	float tempRed = 0.0f;
	float tempGreen = 0.0f;
	float tempBlue = 0.0f;

	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		// Initialise total colours to results from ambient lighting calculation
		SDL_Color ambientLightColour = _transVerts.at(i).GetVertexColour();

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
				_transVerts.at(i).GetVertexNormal();

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
		_transVerts.at(i).SetVertexColour(SDL_Color{ static_cast<unsigned char>(totalRed),
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

void Model3D::CalculateLightingPointGouraud(const vector<LightPoint>& pointLights)
{
	float totalRed = 0.0f;
	float totalGreen = 0.0f;
	float totalBlue = 0.0f;
	float tempRed = 0.0f;
	float tempGreen = 0.0f;
	float tempBlue = 0.0f;

	for (unsigned int i = 0; i < _transVerts.size(); i++)
	{
		// Initialise total colours to results from directional lighting calculations
		SDL_Color directionalLightColour = _transVerts.at(i).GetVertexColour();

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
				// For Gouraud shading we can just use the current vertex, this way
				// the lighting will be calculated for every Vertex
				Vector3D pointLightSource = _transVerts.at(i) - pointLights.at(j).GetLightPosition();

				// Get vector length to light source before we normalise it
				// as we need the length for the attenuation coefficient calculation
				float distanceFromLightSource = pointLightSource.GetVectorLength();

				// Now we can normalise the light source vector
				pointLightSource.Normalise();

				// Now we have the point light source vector, we can calculate
				// the dot product against the polygon normal
				float lightDotProduct = pointLightSource * _transVerts.at(i).GetVertexNormal();

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
		_transVerts.at(i).SetVertexColour(SDL_Color{ static_cast<unsigned char>(totalRed),
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

	for (unsigned int i = 0; i < _UVs.size(); i++)
	{
		_UVs.at(i) = m.GetTexture(i);
	}

	_texture = m.GetTextureMap();
	_palette = m.GetPalette();
	_textureWidth = m.GetTextureMapWidth();

	_worldRotations = m.GetWorldRotations();
	_worldTranslations = m.GetWorldTranslations();
	_worldScales = m.GetWorldScales();
}
