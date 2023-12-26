#pragma once

#ifndef RENDERER_H
#define RENDERER_H 1

class Renderer;

#include <SDL.h>
#include <string>
#include <vector>
#include "Camera.h"
#include "Entity.h"
#include "Surface.h"

class Renderer {
protected:
	static const int default_width;
	static const int default_height;
	int _width;
	int _height;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	Camera* _camera;
	std::vector<Surface*> _objects;
	std::vector<Entity*> _lights;

	Uint32 calcPixelColor(int x, int y);
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& rhs) = delete;

	bool loadScene(const std::string& filename);
	bool render();
	inline SDL_Renderer* getRenderer() { return _renderer; }
};

#endif
