#include "../stdafx.h"
#include "Entity.h"

Entity::Entity(const glm::vec3& position) : _position(position), _backward(-1.0f, 0.0f, 0.0f), _right(0.0f, 0.0f, 1.0f), _up(0.0f, 1.0f, 0.0f) {
	return;
}

Entity::Entity(const glm::vec3& position, const glm::vec3& backward, const glm::vec3& right, const glm::vec3& up) : _position(position), _backward(backward), _right(right), _up(up) {
	return;
}

// Creates 3D Orthonormal Basis aligned with the given yaw and pitch
Entity& Entity::setOrientation(float yaw, float pitch) {
	pitch = 90.0f + (-1.0f * pitch);
	_backward = -1.0f * glm::normalize(glm::vec3(glm::sin(glm::radians(pitch)) * glm::cos(glm::radians(yaw)), glm::cos(glm::radians(pitch)), -1.0f * glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw))));
	_right = glm::normalize(glm::vec3(glm::sin(glm::radians(pitch)) * glm::cos(glm::radians(yaw - 90.0f)), 0.0f, -1.0f * glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw - 90.0f))));
	_up = glm::normalize(glm::cross(_backward, _right));
	return *this;
}
