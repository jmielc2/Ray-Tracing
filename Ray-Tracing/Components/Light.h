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
	Light(const glm::vec3& position, const glm::vec3& color = glm::vec3(255.0f, 255.0f, 255.0f), float intensity = 1.0f);
};

#endif