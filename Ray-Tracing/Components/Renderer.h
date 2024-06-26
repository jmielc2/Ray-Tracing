#pragma once

#ifndef RENDERER_H
#define RENDERER_H 1

class Renderer;

#include <SDL.h>
#include <vector>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Surface.h"

class Renderer {
protected:
	static const int default_width;
	static const int default_height;
	static const float default_ambient_factor;
	static const glm::vec3 default_background_color;
	int _width;
	int _height;
	int _numBounces;
	int _numTrials;
	float _ambient_light_factor;
	glm::vec3 _background_color;
	SDL_Window* _window;
	SDL_Surface* _image;
	Camera* _camera;
	std::vector<Surface*> _objects;
	std::vector<Light*> _lights;

	struct SectionBounds {
		SectionBounds(int min, int max, Renderer* renderer) : yMin(min), yMax(max), renderer(renderer) {}
		int yMin;
		int yMax;
		Renderer* renderer;
	};

	bool configure(const std::string& filename);
	glm::vec3 traceRay(const Ray& ray, int bounces = 1, Surface* source = nullptr);
	static int processSection(void* data);
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& rhs) = delete;

	bool loadScene(const std::string& filename);
	bool render();
	inline SDL_Surface* getImage() { return _image; }
	inline SDL_Window* getWindow() { return _window; }
};

#endif
