#pragma once

#ifndef CAMERA_H
#define CAMERA_H 1

class Camera;

#include "glm.hpp"

class Camera {
protected:
	glm::vec3 _viewpoint;
	glm::vec3 _backward;
	glm::vec3 _right;
	glm::vec3 _up;
public:
	Camera();
	Camera(const glm::vec3& position, float yaw, float pitch);

	void debug() const;
};

#endif