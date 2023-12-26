#include "stdafx.h"
#include "Components/Renderer.h"

int main(int argc, char* argv[]) {
	Renderer renderer;
	if (!renderer.loadScene("scene.txt")) {
		return 1;
	}
	if (!renderer.render()) {
		return 1;
	}
	SDL_Renderer* sdl_renderer = renderer.getRenderer();
	SDL_Event event;
	bool done = false;
	while (!done) {
		SDL_RenderPresent(sdl_renderer);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = true;
				break;
			}
		}
	}
	return 0;
}
