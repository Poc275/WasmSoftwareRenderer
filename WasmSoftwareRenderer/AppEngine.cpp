#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include "Vertex.h"
#include "Matrix3D.h"
#include "MD2Loader.h"
#include "Model3D.h"
#include "Camera.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif // _EMSCRIPTEN__

SDL_Window* window;
SDL_Renderer* renderer;

Model3D cube = Model3D();
Camera cammy = Camera(0, 0, 0, Vertex(0, 0, -50.0f, 1.0f), 640, 480);
float rot = 0;

void DrawWireFrame(const Model3D& model)
{
	int nPolygons = model.GetPolygons();
	Vertex points[3];

	for (int i = 0; i < nPolygons; i++)
	{
		// Get polygon
		Polygon3D poly = model.GetPolygon(i);
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

	cube.SetWorldRotations(Vector3D(0, rot, 0));

	cube.ApplyTransformToLocalVertices(Matrix3D::RotateModel(cube.GetWorldRotations()));
	cube.ApplyTransformToTransformedVertices(Matrix3D::TranslateModel(cube.GetWorldTranslations()));
	cube.ApplyTransformToTransformedVertices(Matrix3D::ScaleModel(cube.GetWorldScales()));

	cube.ApplyTransformToTransformedVertices(cammy.GetViewpointTransformation());
	cube.ApplyTransformToTransformedVertices(cammy.GetPerspectiveTransformation());
	cube.Dehomogenize();
	cube.ApplyTransformToTransformedVertices(cammy.GetScreenTransformation());

	DrawWireFrame(cube);
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
	SDL_CreateWindowAndRenderer(800, 800, 0, &window, &renderer);

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
	cube.ApplyTransformToLocalVertices(Matrix3D::RotateModel(cube.GetWorldRotations()));
	cube.ApplyTransformToTransformedVertices(Matrix3D::TranslateModel(cube.GetWorldTranslations()));
	cube.ApplyTransformToTransformedVertices(Matrix3D::ScaleModel(cube.GetWorldScales()));

	cube.ApplyTransformToTransformedVertices(cammy.GetViewpointTransformation());
	cube.ApplyTransformToTransformedVertices(cammy.GetPerspectiveTransformation());
	cube.Dehomogenize();
	cube.ApplyTransformToTransformedVertices(cammy.GetScreenTransformation());
	DrawWireFrame(cube);

	SDL_RenderPresent(renderer);

	run_main_loop();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
