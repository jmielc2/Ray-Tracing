#include "stdafx.h"
#include <SDL_image.h>
#include "Components/Renderer.h"

int main(int argc, char* argv[]) {
	Renderer renderer;
	if (!renderer.loadScene("scene.txt")) {
		return 1;
	}
	if (!renderer.render()) {
		return 1;
	}
	SDL_Window* window = renderer.getWindow();
	SDL_Event event;
	bool done = false;
	while (!done) {
		int start = Uint32(SDL_GetTicks64());
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
		int delay = int(1000.0f / 30.0f) - (int(SDL_GetTicks64()) - start);
		if (delay > 0) {
			SDL_Delay(delay);
		}
	}
	return 0;
}
