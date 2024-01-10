#include "../stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Surface.h"
#include <SDL_thread.h>

#define NUM_THREADS 8

const int Renderer::default_width = 650;
const int Renderer::default_height = 500;
const float Renderer::default_ambient_factor = 0.1f;
const glm::vec3 Renderer::default_background_color = glm::vec3(0.0f, 0.0f, 0.0f);

static inline Uint32 convertVectorToColor(const glm::vec3& color) {
	return (0xff000000 | (Uint32(color.r) << 16) | (Uint32(color.g) << 8) | Uint32(color.b));
}

Renderer::Renderer() : _width(default_width), _height(default_height), _ambient_light_factor(default_ambient_factor), _background_color(default_background_color), _window(nullptr), _image(nullptr), _camera(nullptr) {
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
	for (Light* light : _lights) {
		delete light;
	}
	delete _camera;
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

glm::vec3 Renderer::traceRay(const Ray& ray, int bounces, Surface* source) {
	// Find closest object of intersection
	Surface* closest = nullptr;
	float t = INFINITY;
	for (Surface* obj : _objects) {
		if (obj == source) {
			continue;
		}
		float intersection = obj->getIntersectionParam(ray);
		if (intersection >= 0.0f && intersection < t) {
			t = intersection;
			closest = obj;
		}
	}

	// Calculate the object's color at point of intersection (if object of intersection exists)
	if (closest) {
		// Phong Shading Implementation
		glm::vec3 point = ray.getPoint(t);
		glm::vec3 normal = closest->getNormal(point);
		glm::vec3 base = closest->getBaseColor(point);
		glm::vec3 color = _ambient_light_factor * base;
		for (Light* light : _lights) {
			glm::vec3 lightDir = light->getLightDirection(point);
			// Check if in shadow
			bool blocked = false;
			for (Surface* obj : _objects) {
				if (obj == closest || obj->getIntersectionParam(Ray(point, lightDir)) < 0.0f) {
					continue;
				}
				blocked = true;
				break;
			}
			if (blocked) {
				continue;
			}
			// Blinn-Phong Shading Calculation
			float intensity = light->getIntensity();
			float phongExp = closest->getPhongExponent();
			glm::vec3 halfway = glm::normalize(-1.0f * ray.getDirection() + lightDir);
			color += ((base * intensity * std::max(0.0f, glm::dot(normal, lightDir))) + (glm::vec3(100.0f, 100.0f, 100.0f) * intensity * std::pow(std::max(0.0f, glm::dot(normal, halfway)), phongExp)));
		}
		// Account for reflectivity of surface
		glm::vec3 reflection = glm::vec3(0.0f, 0.0f, 0.0f);
		float reflectivity = closest->getReflectivity();
		if (bounces && reflectivity > 0.0f) {
			glm::vec3 dir = ray.getDirection();
			glm::vec3 reflectDir = dir - (2 * glm::dot(dir, normal) * normal);
			color = (reflectivity * traceRay(Ray(point, reflectDir, ray.getMaxBound()), bounces - 1, closest)) + ((1.0f - reflectivity) * color);
		}
		return glm::clamp(color, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(255.0f, 255.0f, 255.0f));
	}
	return _background_color;
}

int Renderer::processSection(void* data) {
	SectionBounds* bounds = (SectionBounds*) data;
	Renderer* r = bounds->renderer;
	for (int y = bounds->yMin; y <= bounds->yMax; y++) {
		for (int x = 0; x < r->_image->w; x++) {
			Ray ray = r->_camera->getRay(x, r->_image->h - y);
			((Uint32*)r->_image->pixels)[y * r->_image->w + x] = convertVectorToColor(r->traceRay(ray, 3));
		}
	}
	delete bounds;
	return 0;
}

bool Renderer::loadScene(const std::string& filename) {
	// TODO: Implement scene configuration

	_window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_HIDDEN);
	if (!_window) {
		SDL_Log("SDL failed to create a window: %s\n", SDL_GetError());
		return false;
	}
	_image = SDL_GetWindowSurface(_window);

	/* Configuration is currently hard coded into the loading process. Will be updated later to be loaded from file. */

	// Setup Environment
	_background_color = glm::vec3(0.0f, 0.0f, 0.0f);

	// Setup Camera
	_camera = new PerspCamera(glm::vec3(0.0f, 1.5f, 3.0f), 4.0f, _width, _height, 18.0f);
	// _camera = new OrthoCamera(glm::vec3(0.0f, 3.0f, 3.5f), _width, _height, 15.0f);
	_camera->setOrientation(90.0f, -45.0f, 0.0f);

	// Setup Lights
	_lights.push_back(new Light(glm::vec3(-10.0f, 13.0f, 10.0f)));
	_lights.push_back(new Light(glm::vec3(10.0f, 8.0f, 8.0f), 0.5f));
	_lights.push_back(new Light(glm::vec3(0.0f, 10.0f, 10.0f), 0.7f));

	// Setup Objects
	_objects.push_back(new Sphere(glm::vec3(-1.5f, 1.0f, 1.5f), 1.0f, glm::vec3(30.0f, 151.0f, 225.0f), Surface::SURFACE_TYPES::SHINY));
	_objects.push_back(new Sphere(glm::vec3(1.5f, 1.0f, 2.0f), 1.0f, glm::vec3(100.0f, 100.0f, 100.0f), Surface::SURFACE_TYPES::GLOSSY, 0.85));
	_objects.push_back(new Ground(glm::vec3(255.0f, 255.0f, 255.0f)));
	return true;
}

bool Renderer::render() {
	using namespace std::chrono;
	high_resolution_clock clock;
	steady_clock::time_point start = clock.now();
	SDL_Thread* threads[NUM_THREADS];
	const int span = _image->h / NUM_THREADS;
	for (int i = 0; i < NUM_THREADS; i++) {
		SectionBounds* data = new SectionBounds(i * span, ((i + 1) * span) - 1, this);
		threads[i] = SDL_CreateThread(Renderer::processSection, "Section Thread", data);
		if (!threads[i]) {
			SDL_Log("SDL failed to create thread.");
			std::exit(1);
		}
	}
	for (int i = 0; i < NUM_THREADS; i++) {
		SDL_WaitThread(threads[i], NULL);
	}
	steady_clock::time_point end = clock.now();
	SDL_Log("Render Time - %f ms\n", duration<float, std::milli>(end - start).count());
	SDL_ShowWindow(_window);
	return true;
}