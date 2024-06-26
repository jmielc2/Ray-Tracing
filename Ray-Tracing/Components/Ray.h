#pragma once

#ifndef RAY_H
#define RAY_H 1

class Ray;

#include "glm/glm.hpp"

class Ray {
protected:
	glm::vec3 _start;
	glm::vec3 _direction;
	float _minBound;
	float _maxBound;
public:
	Ray(const glm::vec3& start, const glm::vec3& direction);
	Ray(const glm::vec3& start, const glm::vec3& direction, float maxBound);
	Ray(const glm::vec3& start, const glm::vec3& direction, float minBound, float maxBound);

	inline float getMinBound() const { return _minBound; }
	inline float getMaxBound() const { return _maxBound; }
	inline bool isWithinBounds(float t) const { return (t >= _minBound && t <= _maxBound); }
	inline glm::vec3 getStart() const { return _start; }
	inline glm::vec3 getDirection() const { return _direction; }
	glm::vec3 getPoint(float t) const;
#ifdef _DEBUG
	void debug() const;
#endif
};

#endif
