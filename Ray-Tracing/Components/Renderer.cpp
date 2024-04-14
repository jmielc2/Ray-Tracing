#include "../stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Surface.h"
#include <fstream>
#include <SDL_thread.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define NUM_THREADS 6
#define MIN_DELAY 2
#define MAX_DELAY 8

const int Renderer::default_width = 650;
const int Renderer::default_height = 500;
const float Renderer::default_ambient_factor = 0.1f;
const glm::vec3 Renderer::default_background_color = glm::vec3(0.0f, 0.0f, 0.0f);

static inline Uint32 convertVectorToColor(const glm::vec3& color) {
	return (0xff000000 | (Uint32(color.r) << 16) | (Uint32(color.g) << 8) | Uint32(color.b));
}

Renderer::Renderer() : _width(default_width), _height(default_height), _numBounces(3), _ambient_light_factor(default_ambient_factor), _background_color(default_background_color), _window(nullptr), _image(nullptr), _camera(nullptr) {
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
		if (glm::distance(point, _camera->getPosition()) > _camera->getMaxRenderDist()) {
			return _background_color;
		}
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
		float reflectivity = closest->getReflectivity();
		if (bounces && reflectivity > 0.0f) {
			glm::vec3 dir = ray.getDirection();
			glm::vec3 reflectDir = dir - (2 * glm::dot(dir, normal) * normal);
			color = (reflectivity * traceRay(Ray(point, reflectDir, INFINITY), bounces - 1, closest)) + ((1.0f - reflectivity) * color);
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
			std::vector<Ray> rays = r->_camera->getRays(x, r->_image->h - y);
			glm::vec3 color(0.0f);
			for (Ray& ray : rays) {
				color += r->traceRay(ray, r->_numBounces);
			}
			color /= rays.size();
			((Uint32*)r->_image->pixels)[y * r->_image->w + x] = convertVectorToColor(color);
		}
	}
	delete bounds;
	return 0;
}

bool Renderer::configure(const std::string& filename) {
	try {
		std::ifstream file(filename);
		if (!file.is_open()) {
			SDL_Log("File '%s' not found.", filename.c_str());
			return false;
		}
		json data = json::parse(file);

		// Setup Environment
		{
			json env = data["environment"];
			json bc = env["background_color"];
			_background_color = glm::vec3(bc[0], bc[1], bc[2]);
			_width = env["width"];
			_height = env["height"];
			_numBounces = env["num_bounces"];
		}

		// Setup Camera
		{
			json cam = data["camera"];
			json pos = cam["position"];
			if (cam["type"] == "orthographic") {
				_camera = new OrthoCamera(
					glm::vec3(pos[0], pos[1], pos[2]),
					_width, _height, cam["renderDist"]
				);
			} else if (cam["type"] == "perspective") {
				_camera = new PerspCamera(
					glm::vec3(pos[0], pos[1], pos[2]),
					cam["focalDist"], _width, _height, cam["renderDist"]
				);
			} else {
				SDL_Log("Camera type '%s' is invalid.", cam["type"].get<std::string>().c_str());
				return false;
			}
			json ori = cam["orientation"];
			_camera->setOrientation(ori["yaw"], ori["pitch"], ori["roll"]);
		}
		
		// Setup Lights
		{
			json lights = data["lights"];
			for (const json& light : lights) {
				json pos = light["position"];
				json color = light["color"];
				_lights.push_back(new Light(
					glm::vec3(pos[0], pos[1], pos[2]),
					light["intensity"],
					glm::vec3(color[0], color[1], color[2])
				));
			}
		}

		// Setup Objects
		{
			json objs = data["objects"];
			for (const json& obj : objs) {
				json color = obj["color"];
				Surface::SURFACE_TYPES st;
				if (obj["surfaceType"] == "eggshell") {
					st = Surface::SURFACE_TYPES::EGGSHELL;
				} else if (obj["surfaceType"] == "shiny") {
					st = Surface::SURFACE_TYPES::SHINY;
				} else if (obj["surfaceType"] == "glossy") {
					st = Surface::SURFACE_TYPES::GLOSSY;
				} else if (obj["surfaceType"] == "mirror") {
					st = Surface::SURFACE_TYPES::MIRROR;
				} else {
					SDL_Log("Object surface type '%s' is invalid.", obj["surfaceType"].get<std::string>().c_str());
					return false;
				}
				if (obj["type"] == "sphere") {
					json pos = obj["position"];
					_objects.push_back(new Sphere(
						glm::vec3(pos[0], pos[1], pos[2]),
						obj["radius"],
						glm::vec3(color[0], color[1], color[2]),
						st, obj["reflectivity"]
					));
				} else if(obj["type"] == "ground") {
					_objects.push_back(new Ground(
						glm::vec3(color[0], color[1], color[2]),
						st, obj["reflectivity"]
					));
				} else {
					SDL_Log("Object type '%s' is invalid.", obj["type"].get<std::string>().c_str());
					return false;
				}
			}
		}
	} catch (std::exception e) {
		SDL_Log("Error parsing scene JSON. Make sure your JSON file is formatted properly.");
		return false;
	}
	return true;
}

bool Renderer::loadScene(const std::string& filename) {
	// Configure Renderer from scene description
	if (!this->configure(filename)) {
		return false;
	}

	_window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_HIDDEN);
	if (!_window) {
		SDL_Log("SDL failed to create a window: %s\n", SDL_GetError());
		return false;
	}
	_image = SDL_GetWindowSurface(_window);
	return true;
}

bool Renderer::render() {
	using namespace std::chrono;
	high_resolution_clock clock;
	int numTrials = 1;
	steady_clock::time_point start = clock.now();
	for (int i = 0; i < numTrials; i++) {
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
	}
	steady_clock::time_point end = clock.now();
	SDL_Log("Render Time - %f ms\n", duration<float, std::milli>(end - start).count() / numTrials);
	SDL_ShowWindow(_window);
	return true;
}