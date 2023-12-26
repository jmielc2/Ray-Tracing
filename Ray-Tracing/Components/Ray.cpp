#include "Ray.h"
#include "SDL.h"
#include "glm.hpp"

Ray::Ray(const glm::vec3& start, const glm::vec3& direction) : _start(start), _direction(direction), _minBound(0.0f), _maxBound(INFINITY) {
	return;
}

Ray::Ray(const glm::vec3& start, const glm::vec3& direction, float minBound, float maxBound) : _start(start), _direction(direction), _minBound(minBound), _maxBound(maxBound) {
	return;
}

glm::vec3 Ray::getPoint(float t) const {
	if (_minBound > t || t > _maxBound) {
		SDL_Log("ERROR: getPoint tried to accessed ray point outside of its bounds.");
		std::exit(1);
	}
	return _start + (t * _direction);
}
