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
	for (Surface* obj : _objects) {
		delete obj;
	}
	delete _camera;
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Uint32 Renderer::traceRay(const Ray& ray, int bounces) {
	// Find closest object of intersection
	Surface* closest = nullptr;
	float t = INFINITY;
	for (Surface* obj : _objects) {
		float intersection = obj->getIntersectionParam(ray);
		if (intersection >= 0.0f && intersection < t) {
			t = intersection;
			closest = obj;
		}
	}

	// Calculate the object's color at point of intersection (if object of intersection exists)
	if (closest) {
		glm::vec3 color = closest->getBaseColor(ray.getPoint(t));
		return (0xff000000 | (Uint32(color.r) << 16) | (Uint32(color.g) << 8) | Uint32(color.b));
	}
	return 0xff000000;
}

bool Renderer::loadScene(const std::string& filename) {
	// TODO: Implement scene configuration

	_window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_HIDDEN);
	if (!_window) {
		SDL_Log("SDL failed to create a window: %s\n", SDL_GetError());
		return false;
	}
	_image = SDL_GetWindowSurface(_window);

	// Setup Camera
	_camera = new PerspCamera(glm::vec3(0.0f, 2.0f, 7.0f), 4.0f, _width, _height, 12.5f);
	// _camera = new OrthoCamera(glm::vec3(0.0f, 5.0f, 10.0f), _width, _height, 500.0f);
	_camera->setOrientation(45.0f, -35.0f);

	// Setup Objects
	_objects.push_back(new Ground(glm::vec3(255.0f, 255.0f, 255.0f)));

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
			Ray ray = _camera->getRay(x, _image->h - y);
			((Uint32*)_image->pixels)[y * _image->w + x] = traceRay(ray);
		}
	}
	steady_clock::time_point end = clock.now();
	SDL_Log("Render Time - %f ms\n", duration<float, std::milli>(end - start).count());
	SDL_ShowWindow(_window);
	return true;
}