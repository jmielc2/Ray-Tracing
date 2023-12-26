#include "../stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Entity.h"
#include "Surface.h"
#include <chrono>

const int Renderer::default_width = 650;
const int Renderer::default_height = 500;

Renderer::Renderer() : _width(default_width), _height(default_height), _window(nullptr), _renderer(nullptr), _camera(nullptr) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL failed to initialize: %s\n", SDL_GetError());
		std::exit(1);
	}
	return;
}

Renderer::~Renderer() {
	delete _camera;
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

Uint32 Renderer::calcPixelColor(int x, int y) {
	// TODO: Implement Ray Tracing algorithm
	return 0xffff00ff;
}

bool Renderer::loadScene(const std::string& filename) {
	// TODO: Implement scene configuration

	_window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_HIDDEN);
	if (!_window) {
		SDL_Log("SDL failed to create a window: %s\n", SDL_GetError());
		return false;
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	if (!_renderer) {
		SDL_Log("SDL failed to create a window renderer: %s\n", SDL_GetError());
		return false;
	}
	_camera = new PerspCamera();
	return true;
}

bool Renderer::render() {
	using namespace std::chrono;

	SDL_Surface* image = SDL_CreateRGBSurfaceWithFormat(0, _width, _height, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32);
	for (int y = 0; y < image->h; y++) {
		for (int x = 0; x < image->w; x++) {
			((Uint32*)image->pixels)[y * image->w + x] = calcPixelColor(x, y);
		}
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, image);
	SDL_FreeSurface(image);
	if (!texture) {
		SDL_Log("SDL failed to convert surface to a texture: %s\n", SDL_GetError());
		return false;
	}
	SDL_RenderCopyEx(_renderer, texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
	SDL_DestroyTexture(texture);
	SDL_ShowWindow(_window);
	SDL_RenderPresent(_renderer);
	return true;
}