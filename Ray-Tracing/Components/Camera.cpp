#include "Camera.h"
#include "gtx/transform.hpp"
#include <stdio.h>

Camera::Camera() : _viewpoint(0.0f, 0.0f, 0.0f), _backward(-1.0f, 0.0f, 0.0f), _right(0.0f, 0.0f, 1.0f), _up(0.0f, 1.0f, 0.0f)
{
	return;
}

Camera::Camera(const glm::vec3& position, float yaw, float pitch) :
	_viewpoint(position),
	_backward(glm::normalize(glm::vec3(-1 * glm::cos(glm::radians(yaw)), -1 * glm::sin(glm::radians(pitch)), glm::sin(glm::radians(yaw))))),
	_right(glm::vec3(glm::cos(glm::radians(yaw - 90.0f)), 0.0f, -1 * glm::sin(glm::radians(yaw - 90.0f)))),
	_up(glm::normalize(glm::cross(_backward, _right)))
{
	return;
}

void Camera::debug() const {
	printf("viewpoint = (%f, %f, %f)\n", _viewpoint.x, _viewpoint.y, _viewpoint.z);
	printf("backward  = (%f, %f, %f)\n", _backward.x, _backward.y, _backward.z);
	printf("right     = (%f, %f, %f)\n", _right.x, _right.y, _right.z);
	printf("up        = (%f, %f, %f)\n", _up.x, _up.y, _up.z);
}