#include "Components/Camera.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	PerspCamera camera(glm::vec3(0.0f, 0.0f, 5.0f), 0.5f, 200, 200);
	camera.setOrientation(10.0f, -45.0f);
	camera.debug();
	return 0;
}