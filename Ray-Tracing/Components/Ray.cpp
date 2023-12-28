#include "../stdafx.h"
#include "Ray.h"

#include <stdio.h>

Ray::Ray(const glm::vec3& start, const glm::vec3& direction) : _start(start), _direction(glm::normalize(direction)), _minBound(0.0f), _maxBound(INFINITY) {
	return;
}

Ray::Ray(const glm::vec3& start, const glm::vec3& direction, float maxBound) : Ray(start, direction, 0.0f, maxBound) {
	return;
}

Ray::Ray(const glm::vec3& start, const glm::vec3& direction, float minBound, float maxBound) : _start(start), _direction(glm::normalize(direction)), _minBound(minBound), _maxBound(maxBound) {
	return;
}

glm::vec3 Ray::getPoint(float t) const {
	if (_minBound > t || t > _maxBound) {
		SDL_Log("ERROR: getPoint tried to accessed ray point outside of its bounds.");
		std::exit(1);
	}
	return _start + (t * _direction);
}

#ifdef _DEBUG
void Ray::debug() const {
	printf("start = (%f, %f, %f)\n", _start.x, _start.y, _start.z);
	printf("dir   = (%f, %f, %f)\n", _direction.x, _direction.y, _direction.z);
}
#endif
