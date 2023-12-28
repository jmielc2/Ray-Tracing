#include "../stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Entity.h"
#include "Surface.h"

const int Renderer::default_width = 650;
const int Renderer::default_height = 500;

Renderer::Renderer() : _width(default_width), _height(default_height), _window(nullptr), _image(nullptr), _camera(nullptr) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL failed to initialize: %s\n", SDL_GetError());
		std::exit(1);
	}
	return;
}

Renderer::~Renderer() {
	delete _camera;
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Uint32 Renderer::calcPixelColor(int x, int y) {
	// TODO: Implement Ray Tracing algorithm
	Ray ray = _camera->getRay(x, y);
	return 0xffff00ff;
}

bool Renderer::loadScene(const std::string& filename) {
	// TODO: Implement scene configuration

	_window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_HIDDEN);
	if (!_window) {
		SDL_Log("SDL failed to create a window: %s\n", SDL_GetError());
		return false;
	}
	_image = SDL_GetWindowSurface(_window);
	_camera = new PerspCamera(glm::vec3(0.0f, 5.0f, 10.0f), 4.0f, _width, _height);
	// _camera = new OrthoCamera(glm::vec3(0.0f, 5.0f, 10.0f), _width, _height);
	_camera->setOrientation(90.0f, -10.0f);
#ifdef _DEBUG
	_camera->debug();
#endif
	return true;
}

bool Renderer::render() {
	using namespace std::chrono;
	high_resolution_clock clock;
	steady_clock::time_point start = clock.now();
	for (int y = 0; y < _image->h; y++) {
		for (int x = 0; x < _image->w; x++) {
			((Uint32*)_image->pixels)[y * _image->w + x] = calcPixelColor(x, _image->h - y);
		}
	}
	steady_clock::time_point end = clock.now();
	SDL_Log("Render Time - %f ms\n", duration<float, std::milli>(end - start).count());
	SDL_ShowWindow(_window);
	return true;
}