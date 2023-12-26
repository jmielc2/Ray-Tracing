#include "../stdafx.h"
#include "Camera.h"
#include <stdio.h>

/* Abstract Camera Class */

Camera::Camera(const glm::vec3& position) : Entity(position), _imgViewX(200), _imgViewY(200) {
	return;
}

Camera::Camera(const glm::vec3& position, int x, int y) : Entity(position), _imgViewX(x), _imgViewY(y) {
	return;
}

void Camera::debug() const {
	printf("Img View Center = (%f, %f, %f)\n", _position.x, _position.y, _position.z);
	printf("backward  = (%f, %f, %f)\n", _backward.x, _backward.y, _backward.z);
	printf("right     = (%f, %f, %f)\n", _right.x, _right.y, _right.z);
	printf("up        = (%f, %f, %f)\n", _up.x, _up.y, _up.z);
}

/* Orthogonal Camera Class */

OrthoCamera::OrthoCamera() : Camera(glm::vec3(0.0f, 0.0f, 5.0f)) {
	return;
}

OrthoCamera::OrthoCamera(const glm::vec3& position) : Camera(position) {
	return;
}

OrthoCamera::OrthoCamera(const glm::vec3& position, int x, int y) : Camera(position, x, y) {
	return;
}

Ray OrthoCamera::getRay(int pixelX, int pixelY) const {
	// TODO: Compute the ray for the given pixel
	return Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

/* Perspective Camera Class */

PerspCamera::PerspCamera() : Camera(glm::vec3(0.0f, 0.0f, 5.0f)), _viewpoint(_position + (0.5f * _backward)), _focalDist(0.5f) {
	return;
}

PerspCamera::PerspCamera(const glm::vec3& position, float focalDist) : Camera(position), _viewpoint(_position + (_backward * focalDist)), _focalDist(focalDist) {
	return;
}

PerspCamera::PerspCamera(const glm::vec3& position, float focalDist, int x, int y) : Camera(position, x, y), _viewpoint(_position + (_backward * focalDist)), _focalDist(focalDist) {
	return;
}

PerspCamera& PerspCamera::setOrientation(float yaw, float pitch) {
	Camera::setOrientation(yaw, pitch);
	_viewpoint = _position + (_backward * _focalDist);
	return *this;
}

Ray PerspCamera::getRay(int pixelX, int pixelY) const {
	// TODO: Compute the ray for the given pixel
	return Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void PerspCamera::debug() const {
	printf("Viewpoint: (%f, %f, %f)\n", _viewpoint.x, _viewpoint.y, _viewpoint.z);
	Camera::debug();
}
