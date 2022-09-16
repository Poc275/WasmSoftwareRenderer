#include <SDL.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif // _EMSCRIPTEN__
#include "Vertex.h"

SDL_Window* window;
SDL_Renderer* renderer;

bool handle_events()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
	{
		return false;
	}
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
	SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);

	/*SDL_Rect rect{};
	rect.x = 10;
	rect.y = 10;
	rect.w = 150;
	rect.h = 100;

	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);

	run_main_loop();*/

	Vertex verts[4];
	verts[0] = Vertex(200, 200, 0, 1);
	verts[1] = Vertex(250, 200, 0, 1);
	verts[2] = Vertex(250, 250, 0, 1);
	verts[3] = Vertex(200, 250, 0, 1);

	SDL_RenderDrawLineF(renderer, verts[0].GetX(), verts[0].GetY(), verts[1].GetX(), verts[1].GetY());
	SDL_RenderDrawLineF(renderer, verts[1].GetX(), verts[1].GetY(), verts[2].GetX(), verts[2].GetY());
	SDL_RenderDrawLineF(renderer, verts[2].GetX(), verts[2].GetY(), verts[3].GetX(), verts[3].GetY());
	SDL_RenderDrawLineF(renderer, verts[3].GetX(), verts[3].GetY(), verts[0].GetX(), verts[0].GetY());

	SDL_RenderPresent(renderer);

	run_main_loop();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
