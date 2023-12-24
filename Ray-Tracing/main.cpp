#include "Components/Camera.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	glm::vec3 pos(0.0f, 0.0f, 3.0f);
	Camera camera(pos, 90.0f, 0.0f);
	camera.debug();
	SDL_Quit();
	return 0;
}