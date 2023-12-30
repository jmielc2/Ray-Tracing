#pragma once

#ifndef LIGHT_H
#define LIGHT_H 1

class Light;

#include "Entity.h"
#include "glm.hpp"

class Light : public Entity {
protected:
	glm::vec3 _lightColor;
	float _intensity;
public:
	Light(const glm::vec3& position, float intensity = 1.0f, const glm::vec3& color = glm::vec3(255.0f, 255.0f, 255.0f));
	inline glm::vec3 getLightColor() { return _lightColor; }
	inline glm::vec3 getLightDirection(const glm::vec3& point) { return glm::normalize(_position - point); }
	inline float getIntensity() { return _intensity; }
};

#endif