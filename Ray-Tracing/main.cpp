#include "stdafx.h"
#include <SDL_image.h>
#include "Components/Renderer.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		SDL_Log("Usage: %s <filename>", argv[0]);
		return 0;
	}
	Renderer renderer;
	if (!renderer.loadScene(std::string(argv[1]))) {
		return 1;
	}
	if (!renderer.render()) {
		return 1;
	}
	SDL_Window* window = renderer.getWindow();
	SDL_Event event;
	bool done = false;
	while (!done) {
		Uint64 start = SDL_GetTicks64();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_s) {
					if (!IMG_SavePNG(renderer.getImage(), "rendered-scene.png")) {
						SDL_Log("Image saved to 'rendered-scene.png'\n");
					} else {
						SDL_Log("Image couldn't be saved.\n");
					}
				}
				break;
			}
		}
		SDL_UpdateWindowSurface(window);
		int delay = int(1000.0f / 30.0f) - int(SDL_GetTicks64() - start);
		if (delay > 0) {
			SDL_Delay(delay);
		}
	}
	return 0;
}
