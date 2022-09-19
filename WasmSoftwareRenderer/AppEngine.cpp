#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "Vertex.h"
#include "Matrix3D.h"
#include "MD2Loader.h"
#include "Model3D.h"
#include "Camera.h"
#include "LightAmbient.h"
#include "LightPoint.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif // _EMSCRIPTEN__

using namespace std;

struct ScanLine
{
	float xStart = 0;
	float xEnd = 0;
	SDL_Color startColour = SDL_Color{ 0, 0, 0, 255 };
	SDL_Color endColor = SDL_Color{ 0, 0, 0, 255 };
	Texture startTexture;
	Texture endTexture;
	/*float startFogDensity;
	float endFogDensity;
	float startUOverZ;
	float endUOverZ;
	float startVOverZ;
	float endVOverZ;
	float startZRecip;
	float endZRecip;*/
	Vector3D startNormal;
	Vector3D endNormal;
};
ScanLine _scanLines[800];

unsigned int _width = 800;
unsigned int _height = 800;

SDL_Window* window;
SDL_Renderer* renderer;

Model3D cube = Model3D();
Model3D marvin = Model3D();
Camera cammy = Camera();
float rot = 0;

// LIGHTS
LightAmbient ambientLight = LightAmbient(0.0f, 188.0f, 188.0f, 0.2f, 0.2f, 0.2f);
vector<LightDirectional> directionalLights;
vector<LightPoint> pointLights;

void DrawWireFrame(const Model3D& model)
{
	int nPolygons = model.GetPolygons();
	Vertex points[3];

	for (int i = 0; i < nPolygons; i++)
	{
		// Get polygon
		Polygon3D poly = model.GetPolygon(i);

		// only draw if not backward facing
		if (poly.DrawPolygon())
		{
			for (int j = 0; j < 3; j++)
			{
				// Get polygon index
				int polyIndex = poly.GetVertexIndex(j);

				// Get vertex from index
				Vertex vert = model.GetTransformedVertex(polyIndex);

				// Populate vertices array with X and Y values
				points[j].SetX(vert.GetX());
				points[j].SetY(vert.GetY());
			}

			// Draw the polygon now we have the 3 points
			for (int j = 0; j < 3; j++)
			{
				if (j == 2)
				{
					// We are at the last vertex, so draw line back to the beginning
					SDL_RenderDrawLineF(renderer, points[j].GetX(), points[j].GetY(), points[0].GetX(), points[0].GetY());
				}
				else
				{
					SDL_RenderDrawLineF(renderer, points[j].GetX(), points[j].GetY(), points[j + 1].GetX(), points[j + 1].GetY());
				}
			}
		}
	}
}

void DrawSolidFlat(const Model3D& model)
{
	int nPolygons = model.GetPolygons();
	SDL_Vertex verts[3];

	for (int i = 0; i < nPolygons; i++)
	{
		// Get polygon
		Polygon3D poly = model.GetPolygon(i);

		// only draw if not backward facing
		if (poly.DrawPolygon())
		{
			for (int j = 0; j < 3; j++)
			{
				// Get polygon index
				int polyIndex = poly.GetVertexIndex(j);

				// Get vertex from index
				Vertex vert = model.GetTransformedVertex(polyIndex);

				// Create an SDL_Vertex from the Vertex info
				verts[j] = SDL_Vertex{ SDL_FPoint{ vert.GetX(), vert.GetY() }, poly.GetLightingColour(), SDL_FPoint{0}};
			}

			SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
		}
	}
}

void DrawSolidGouraud(const Model3D& model)
{
	int nPolygons = model.GetPolygons();
	SDL_Vertex verts[3];

	for (int i = 0; i < nPolygons; i++)
	{
		// Get polygon
		Polygon3D poly = model.GetPolygon(i);

		// only draw if not backward facing
		if (poly.DrawPolygon())
		{
			for (int j = 0; j < 3; j++)
			{
				// Get polygon index
				int polyIndex = poly.GetVertexIndex(j);

				// Get vertex from index
				Vertex vert = model.GetTransformedVertex(polyIndex);

				// Create an SDL_Vertex from the Vertex info
				// Only difference here is that we're using the vertex colour, not the polygon colour
				// SDL will interpolate the colour across the polygon for us (cheat!)
				verts[j] = SDL_Vertex{ SDL_FPoint{ vert.GetX(), vert.GetY() }, vert.GetVertexColour(), SDL_FPoint{0}};
			}

			SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
		}
	}
}

void FillSolidTextured(const Model3D& model, Vertex& xStart, Vertex& xEnd, Vertex& y, SDL_Color& startColour, SDL_Color& endColour,
	Texture& startTexture, Texture& endTexture)
{
	int startingX = (int)xStart.GetX();
	int endingX = (int)xEnd.GetX();
	int pixelsInRow = endingX - startingX;
	float tempRed = startColour.r;
	float tempGreen = startColour.g;
	float tempBlue = startColour.b;
	float redInterpolation = (float)endColour.r - (float)startColour.r;
	float greenInterpolation = (float)endColour.g - (float)startColour.g;
	float blueInterpolation = (float)endColour.b - (float)startColour.b;
	float diffU = (float)endTexture.GetTextureU() - (float)startTexture.GetTextureU();
	float diffV = (float)endTexture.GetTextureV() - (float)startTexture.GetTextureV();
	float tempU = startTexture.GetTextureU();
	float tempV = startTexture.GetTextureV();

	// Only interpolate lighting colours & UVs if no of pixels > 0
	// otherwise we are dividing by zero which will crash
	if (pixelsInRow > 0)
	{
		redInterpolation /= pixelsInRow;
		greenInterpolation /= pixelsInRow;
		blueInterpolation /= pixelsInRow;
		diffU /= pixelsInRow;
		diffV /= pixelsInRow;
	}

	for (int i = startingX; i <= endingX; i++)
	{
		int paletteColourIndex = (int)tempV * model.GetTextureMapWidth() + (int)tempU;

		SDL_Color pixelColour = model.GetPaletteColour(paletteColourIndex);

		// Get RGB values from colour retrieved from palette
		float texR = pixelColour.r;
		float texG = pixelColour.g;
		float texB = pixelColour.b;

		// Modulate texture colour with lighting effects
		float finalRed = texR * tempRed / 100;
		float finalGreen = texG * tempGreen / 100;
		float finalBlue = texB * tempBlue / 100;

		// Clamp final colours between 0-255
		if (finalRed < 0 || finalRed > 255)
		{
			if (finalRed < 0)
			{
				finalRed = 0.0f;
			}
			else
			{
				finalRed = 255.0f;
			}
		}

		if (finalGreen < 0 || finalGreen > 255)
		{
			if (finalGreen < 0)
			{
				finalGreen = 0.0f;
			}
			else
			{
				finalGreen = 255.0f;
			}
		}

		if (finalBlue < 0 || finalBlue > 255)
		{
			if (finalBlue < 0)
			{
				finalBlue = 0.0f;
			}
			else
			{
				finalBlue = 255.0f;
			}
		}

		//Color finalColour(255, (int)finalRed, (int)finalGreen, (int)finalBlue);
		SDL_Color finalColour = SDL_Color{ static_cast<unsigned char>(finalRed),
										   static_cast<unsigned char>(finalGreen),
										   static_cast<unsigned char>(finalBlue),
										   255 };

		//_bitmap->SetPixel(i, (int)y.GetY(), finalColour);
		SDL_SetRenderDrawColor(renderer, finalColour.r, finalColour.g, finalColour.b, 255);
		SDL_RenderDrawPoint(renderer, i, (int)y.GetY());

		// Interpolate colours
		tempRed += redInterpolation;
		tempGreen += greenInterpolation;
		tempBlue += blueInterpolation;

		// Interpolate UVs
		tempU += diffU;
		tempV += diffV;
	}
}

void DrawSolidTextured(const Model3D& model)
{
	Vertex verts[3];

	for (int i = 0; i < model.GetPolygons(); i++)
	{
		// Get polygon
		Polygon3D poly = model.GetPolygon(i);

		// Only draw polygons that aren't backward facing
		if (poly.DrawPolygon())
		{
			bool horizontalLine = false;

			for (int j = 0; j < 3; j++)
			{
				// Get Polygon index
				int polyIndex = poly.GetVertexIndex(j);

				// Get Vertex from index
				Vertex vert = model.GetTransformedVertex(polyIndex);

				// Clip points in 2D space
				if (vert.GetX() < 0)
				{
					vert.SetX(0.0f);
				}
				if (vert.GetX() > _width)
				{
					vert.SetX((float)_width);
				}

				if (vert.GetY() < 0)
				{
					vert.SetY(0.0f);
				}
				if (vert.GetY() > _height)
				{
					vert.SetY((float)_height);
				}

				// Get Texture from index
				int textureIndex = poly.GetTextureIndex(j);
				Texture texture = model.GetTexture(textureIndex);

				// Set Texture to vert
				vert.SetVertexTexture(texture);

				// Populate array of verts ready for interpolation calcs
				verts[j] = vert;
			}

			// SORT VERTICES
			// Re-arrange verts[] from smallestY to largestY so we know
			// verts[0] is at smallestY and verts[2] is at largestY
			bool swap = true;

			while (swap)
			{
				for (int j = 0; j < 2; j++)
				{
					if (verts[j + 1].GetY() < verts[j].GetY())
					{
						Vertex swappedVert = verts[j];
						verts[j] = verts[j + 1];
						verts[j + 1] = swappedVert;
						swap = true;
					}
					else
					{
						swap = false;
					}
				}
			}

			// Now check if any verts are at the same Y position
			// If so, sort from X left to right
			// We only need to do one pass, as the previous sort will
			// have placed same Y value verts next to each other
			for (int j = 0; j < 2; j++)
			{
				if (verts[j + 1].GetY() == verts[j].GetY())
				{
					// Sort by X left -> right
					if (verts[j + 1].GetX() < verts[j].GetX())
					{
						// Swap
						Vertex swappedVert = verts[j];
						verts[j] = verts[j + 1];
						verts[j + 1] = swappedVert;
					}
				}
			}

			// Calculate total no of spans and round up
			int nSpans = (int)ceil(verts[2].GetY() - verts[0].GetY());

			// Initialise scan lines struct values
			// for total number of spans, this is to enable
			// the x start/end value checks to work
			// Also initialise colours
			for (int j = 0; j < nSpans; j++)
			{
				_scanLines[j].xStart = (float)_width;
				_scanLines[j].xEnd = 0.0f;
				_scanLines[j].startColour = SDL_Color{ 0, 0, 0, 255 };
				_scanLines[j].endColor = SDL_Color{ 0, 0, 0, 255 };
			}

			// For each edge of polygon
			for (int j = 0; j < 3; j++)
			{
				// Values for starting point
				float currentX = 0.0f;
				int startingVertex = 0;
				int endingVertex = 0;

				// start and end colour RGB values
				// and interpolation factors
				SDL_Color startVertexColour;
				SDL_Color endVertexColour;
				float startRed = 0.0f;
				float startGreen = 0.0f;
				float startBlue = 0.0f;
				float endRed = 0.0f;
				float endGreen = 0.0f;
				float endBlue = 0.0f;
				float redInterp = 0.0f;
				float greenInterp = 0.0f;
				float blueInterp = 0.0f;

				// Calculate gradient of line
				float gradient = 0.0f;
				float differenceX = 0.0f;
				float differenceY = 0.0f;

				// Get start and end vert indexes/colours based on which
				// edge of the polygon we're looking at in the for loop
				if (j == 2)
				{
					// Last line of polygon, so go back to verts[0]
					differenceX = verts[j].GetX() - verts[0].GetX();
					differenceY = verts[j].GetY() - verts[0].GetY();

					if (verts[j].GetY() < verts[0].GetY())
					{
						startingVertex = j;
						endingVertex = 0;
						startVertexColour = verts[j].GetVertexColour();
						endVertexColour = verts[0].GetVertexColour();
					}
					else
					{
						startingVertex = 0;
						endingVertex = j;
						startVertexColour = verts[0].GetVertexColour();
						endVertexColour = verts[j].GetVertexColour();
					}
				}
				else
				{
					differenceX = verts[j + 1].GetX() - verts[j].GetX();
					differenceY = verts[j + 1].GetY() - verts[j].GetY();

					if (verts[j + 1].GetY() < verts[j].GetY())
					{
						startingVertex = j + 1;
						endingVertex = j;
						startVertexColour = verts[j + 1].GetVertexColour();
						endVertexColour = verts[j].GetVertexColour();
					}
					else
					{
						startingVertex = j;
						endingVertex = j + 1;
						startVertexColour = verts[j].GetVertexColour();
						endVertexColour = verts[j + 1].GetVertexColour();
					}
				}

				currentX = verts[startingVertex].GetX();

				// Get no of edge spans for this edge
				int edgeSpans = (int)ceil(fabs(differenceY));

				// If differenceY is 0 then the gradient will be undefined
				// this will result in a horizontal line which we can ignore
				if (differenceY != 0.0f)
				{
					gradient = differenceX / differenceY;
				}
				else
				{
					horizontalLine = true;
				}

				// Calculate colour interpolation
				startRed = startVertexColour.r;
				startGreen = startVertexColour.g;
				startBlue = startVertexColour.b;
				endRed = endVertexColour.r;
				endGreen = endVertexColour.g;
				endBlue = endVertexColour.b;
				redInterp = (endRed - startRed) / edgeSpans;
				greenInterp = (endGreen - startGreen) / edgeSpans;
				blueInterp = (endBlue - startBlue) / edgeSpans;

				// UV interpolation
				// Calculate U/V interpolation factors
				float diffU = (float)(verts[endingVertex].GetVertexTexture().GetTextureU() -
					verts[startingVertex].GetVertexTexture().GetTextureU());
				float diffV = (float)(verts[endingVertex].GetVertexTexture().GetTextureV() -
					verts[startingVertex].GetVertexTexture().GetTextureV());
				diffU /= edgeSpans;
				diffV /= edgeSpans;

				// Store starting vertex U/V values which we will increase
				// by the interpolation factor each time
				float tempU = verts[startingVertex].GetVertexTexture().GetTextureU();
				float tempV = verts[startingVertex].GetVertexTexture().GetTextureV();

				// For each scan line, only if it's not a horizontal line
				if (!horizontalLine)
				{
					// Check where to start and end for Y
					if (verts[startingVertex].GetY() == verts[0].GetY())
					{
						for (int k = 0; k < edgeSpans; k++)
						{
							// Check if currentX value is less or more than scan lines
							// current values
							if (currentX > _scanLines[k].xEnd)
							{
								_scanLines[k].xEnd = currentX;
								//_scanLines[k].endColor = Color(255, (int)startRed, (int)startGreen, (int)startBlue);
								_scanLines[k].endColor = SDL_Color{ static_cast<unsigned char>(startRed),
																	static_cast<unsigned char>(startGreen),
																	static_cast<unsigned char>(startBlue),
																	255 };

								_scanLines[k].endTexture.SetTextureU((short)tempU);
								_scanLines[k].endTexture.SetTextureV((short)tempV);
							}
							if (currentX < _scanLines[k].xStart)
							{
								_scanLines[k].xStart = currentX;
								//_scanLines[k].startColour = Color(255, (int)startRed, (int)startGreen, (int)startBlue);
								_scanLines[k].startColour = SDL_Color{ static_cast<unsigned char>(startRed),
																	   static_cast<unsigned char>(startGreen),
																	   static_cast<unsigned char>(startBlue),
																	   255 };
								_scanLines[k].startTexture.SetTextureU((short)tempU);
								_scanLines[k].startTexture.SetTextureV((short)tempV);
							}

							// Interpolate X value
							currentX += gradient;

							// Interpolate colours
							startRed += redInterp;
							startGreen += greenInterp;
							startBlue += blueInterp;

							// Interpolate UVs
							tempU += diffU;
							tempV += diffV;
						}
					}

					else if (verts[startingVertex].GetY() > verts[0].GetY())
					{
						// Current edge Y value is > than smallestY so we can't
						// start edge scanning from zero
						int currentYPos = (int)floor(verts[startingVertex].GetY());
						int startingPosition = currentYPos - (int)floor(verts[0].GetY());
						// We need to update how many edge spans to scan because we aren't
						// starting from zero
						int updatedSpans = startingPosition + edgeSpans;

						for (int k = startingPosition; k < updatedSpans; k++)
						{
							// Check if currentX value is less or more than scan lines
							// current values
							if (currentX > _scanLines[k].xEnd)
							{
								_scanLines[k].xEnd = currentX;
								//_scanLines[k].endColor = Color(255, (int)startRed, (int)startGreen, (int)startBlue);
								_scanLines[k].endColor = SDL_Color{ static_cast<unsigned char>(startRed),
																	static_cast<unsigned char>(startGreen),
																	static_cast<unsigned char>(startBlue),
																	255 };
								_scanLines[k].endTexture.SetTextureU((short)tempU);
								_scanLines[k].endTexture.SetTextureV((short)tempV);
							}
							if (currentX < _scanLines[k].xStart)
							{
								_scanLines[k].xStart = currentX;
								//_scanLines[k].startColour = Color(255, (int)startRed, (int)startGreen, (int)startBlue);
								_scanLines[k].startColour = SDL_Color{ static_cast<unsigned char>(startRed),
																	   static_cast<unsigned char>(startGreen),
																	   static_cast<unsigned char>(startBlue),
																	   255 };
								_scanLines[k].startTexture.SetTextureU((short)tempU);
								_scanLines[k].startTexture.SetTextureV((short)tempV);
							}

							// Interpolate X value
							currentX += gradient;

							// Interpolate colours
							startRed += redInterp;
							startGreen += greenInterp;
							startBlue += blueInterp;

							// Interpolate UVs
							tempU += diffU;
							tempV += diffV;
						}
					}

					else
					{
						// Current edges starting Y value should never be less
						// than the smallestY value for the polygon if the check works properly
						// so do nothing
					}

				}
				// Reset horizontal line bool
				horizontalLine = false;
			}

			// Fill the polygon
			for (int j = 0; j < nSpans; j++)
			{
				Vertex startX(_scanLines[j].xStart, 0.0f, 1.0f, 0.0f);
				Vertex endX(_scanLines[j].xEnd, 0.0f, 1.0f, 0.0f);
				Vertex row(0.0f, verts[0].GetY() + j, 1.0f, 0.0f);

				FillSolidTextured(model, startX, endX, row, _scanLines[j].startColour, _scanLines[j].endColor,
					_scanLines[j].startTexture, _scanLines[j].endTexture);
			}
		}
	}
}

bool handle_events()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
	{
		return false;
	}

	// rotate model test
	rot += 0.001f;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);

	marvin.SetWorldRotations(Vector3D(0, rot, 0));

	marvin.ApplyTransformToLocalVertices(Matrix3D::RotateModel(marvin.GetWorldRotations()));
	marvin.ApplyTransformToTransformedVertices(Matrix3D::TranslateModel(marvin.GetWorldTranslations()));
	marvin.ApplyTransformToTransformedVertices(Matrix3D::ScaleModel(marvin.GetWorldScales()));

	// backface culling
	marvin.CalculateBackfaces(cammy.GetPosition());

	// calculate vertex normals for Gouraud shading
	marvin.CalculateVertexNormals();

	// lighting
	marvin.CalculateLightingAmbient(ambientLight);
	//marvin.CalculateLightingDirectional(directionalLights);
	//marvin.CalculateLightingPoint(pointLights);
	
	// lighting for Gouraud shading (ambient doesn't rely on normals, is just "flat" shading)
	marvin.CalculateLightingDirectionalGouraud(directionalLights);
	marvin.CalculateLightingPointGouraud(pointLights);

	marvin.ApplyTransformToTransformedVertices(cammy.GetViewpointTransformation());

	// polygon sorting
	marvin.Sort();

	marvin.ApplyTransformToTransformedVertices(cammy.GetPerspectiveTransformation());
	marvin.Dehomogenize();
	marvin.ApplyTransformToTransformedVertices(cammy.GetScreenTransformation());

	//DrawWireFrame(marvin);
	//DrawSolidFlat(marvin);
	//DrawSolidGouraud(marvin);
	DrawSolidTextured(marvin);

	SDL_RenderPresent(renderer);

	return true;
}

void run_main_loop()
{
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop([]() { handle_events(); }, 0, true);
#else
	while (handle_events())
		;
	
#endif // __EMSCRIPTEN__
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(_width, _height, 0, &window, &renderer);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);

	/*SDL_Rect rect{};
	rect.x = 10;
	rect.y = 10;
	rect.w = 150;
	rect.h = 100;

	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);

	run_main_loop();*/

	//Vertex verts[4];
	//verts[0] = Vertex(200, 200, 0, 1);
	//verts[1] = Vertex(250, 200, 0, 1);
	//verts[2] = Vertex(250, 250, 0, 1);
	//verts[3] = Vertex(200, 250, 0, 1);

	//SDL_RenderDrawLineF(renderer, verts[0].GetX(), verts[0].GetY(), verts[1].GetX(), verts[1].GetY());
	//SDL_RenderDrawLineF(renderer, verts[1].GetX(), verts[1].GetY(), verts[2].GetX(), verts[2].GetY());
	//SDL_RenderDrawLineF(renderer, verts[2].GetX(), verts[2].GetY(), verts[3].GetX(), verts[3].GetY());
	//SDL_RenderDrawLineF(renderer, verts[3].GetX(), verts[3].GetY(), verts[0].GetX(), verts[0].GetY());

	// scale and rotate about a point (top left corner of square)
	/*Matrix3D loopRotate = Matrix3D(cos(0.25f), -sin(0.25f), 0, 0,
								   sin(0.25f), cos(0.25f), 0, 0,
								   0, 0, 1.0f, 0,
								   0, 0, 0, 1.0f);

	Matrix3D loopScale = Matrix3D(1.05f, 0, 0, 0,
								  0, 1.05f, 0, 0,
								  0, 0, 1.05f, 0,
								  0, 0, 0, 1.0f);

	Matrix3D moveToOrigin = Matrix3D(1.0f, 0, 0, -200.0f,
									 0, 1.0f, 0, -200.0f,
									 0, 0, 1.0f, 0,
									 0, 0, 0, 1.0f);

	Matrix3D moveBack = Matrix3D(1.0f, 0, 0, 200.0f,
								 0, 1.0f, 0, 200.0f,
								 0, 0, 1.0f, 0,
								 0, 0, 0, 1.0f);

	for (int i = 0; i < 20; i++)
	{
		for (int i = 0; i < 4; i++)
		{
			verts[i] = moveBack * loopRotate * loopScale * moveToOrigin * verts[i];
		}

		SDL_RenderDrawLineF(renderer, verts[0].GetX(), verts[0].GetY(), verts[1].GetX(), verts[1].GetY());
		SDL_RenderDrawLineF(renderer, verts[1].GetX(), verts[1].GetY(), verts[2].GetX(), verts[2].GetY());
		SDL_RenderDrawLineF(renderer, verts[2].GetX(), verts[2].GetY(), verts[3].GetX(), verts[3].GetY());
		SDL_RenderDrawLineF(renderer, verts[3].GetX(), verts[3].GetY(), verts[0].GetX(), verts[0].GetY());
	}*/

	// Wireframe models testing...
	MD2Loader::LoadModel("assets/cube.md2", cube);
	MD2Loader::LoadModel("assets/marvin.md2", marvin, "assets/marvin.pcx");

	// Lighting setup
	Vector3D lightDirection(1.0f, 0.0f, 1.0f);
	LightDirectional directLight(0.0f, 255.0f, 255.0f, lightDirection);
	directionalLights.push_back(directLight);

	Vertex pointLightPosition(0.0f, 0.0f, -20.0f, 0.0f);
	LightPoint pointLight(255.0f, 255.0f, 0.0f, pointLightPosition, 0.0f, 1.0f, 0.0f, true);
	pointLights.push_back(pointLight);

	run_main_loop();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
