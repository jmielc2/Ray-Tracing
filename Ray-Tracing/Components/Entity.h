#pragma once

#ifndef ENTITY_H
#define ENTITY_H 1

class Entity;

#include "glm/glm.hpp"

// By default, an entity will start by facing to the right (positive x axis)

class Entity {
protected:
	glm::vec3 _position;
	glm::vec3 _backward;
	glm::vec3 _right;
	glm::vec3 _up;
	Entity(const glm::vec3& position);
	virtual ~Entity();
public:
	virtual Entity& setOrientation(float yaw, float pitch, float roll = 0.0f);
	inline glm::vec3 getPosition() const { return _position; }
};

#endif
