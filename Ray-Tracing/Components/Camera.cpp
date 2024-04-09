#include "../stdafx.h"
#include "Camera.h"
#include "Ray.h"

#include <stdio.h>

/* Abstract Camera Class */

Camera::Camera(const glm::vec3& position, int x, int y, float renderDist) : Entity(position), _imgViewX(x), _imgViewY(y), _renderDist(renderDist) {
	return;
}

Camera::~Camera() {
	return;
}

#ifdef _DEBUG
void Camera::debug() const {
	printf("position  = (%f, %f, %f)\n", _position.x, _position.y, _position.z);
	printf("backward  = (%f, %f, %f)\n", _backward.x, _backward.y, _backward.z);
	printf("right     = (%f, %f, %f)\n", _right.x, _right.y, _right.z);
	printf("up        = (%f, %f, %f)\n", _up.x, _up.y, _up.z);
}
#endif

/* Orthogonal Camera Class */

OrthoCamera::OrthoCamera(const glm::vec3& position, int x, int y, float renderDist) : Camera(position, x, y, renderDist) {
	return;
}

Ray OrthoCamera::getRay(int pixelX, int pixelY) const {
	float x = (_imgViewX / -200.0f) + ((float(pixelX) + 0.5f) / 100.0f);
	float y = (_imgViewY / -200.0f) + ((float(pixelY) + 0.5f) / 100.0f);
	return Ray(_position + (x * _right) + (y * _up), -1.0f * _backward, _renderDist);
}

std::vector<Ray> OrthoCamera::getRays(int pixelX, int pixelY) const {
	std::vector<Ray> rays;
	for (float i = 0.25f; i <= 1.0f; i += 0.25f) {
		for (float j = 0.25f; j <= 1.0f; j += 0.25f) {
			float x = (_imgViewX / -200.0f) + ((float(pixelX) + i) / 100.0f);
			float y = (_imgViewY / -200.0f) + ((float(pixelY) + j) / 100.0f);
			rays.push_back(Ray(_position + (x * _right) + (y * _up), -1.0f * _backward, _renderDist));
		}
	}
	return rays;
}

/* Perspective Camera Class */

PerspCamera::PerspCamera(const glm::vec3& position, float focalDist, int x, int y, float renderDist) : Camera(position, x, y, renderDist), _viewpoint(_position + (_backward * focalDist)), _focalDist(focalDist) {
	return;
}

PerspCamera& PerspCamera::setOrientation(float yaw, float pitch, float roll) {
	Camera::setOrientation(yaw, pitch, roll);
	_viewpoint = _position + (_backward * _focalDist);
	return *this;
}

Ray PerspCamera::getRay(int pixelX, int pixelY) const {
	float x = (_imgViewX / -200.0f) + (pixelX / 100.0f);
	float y = (_imgViewY / -200.0f) + (pixelY / 100.0f);
	glm::vec3 start = _position + (x * _right) + (y * _up);
	return Ray(_viewpoint, start - _viewpoint, _renderDist);
}

std::vector<Ray> PerspCamera::getRays(int pixelX, int pixelY) const {
	std::vector<Ray> rays;
	for (float i = 0.25f; i <= 1.0f; i += 0.25f) {
		for (float j = 0.25f; j <= 1.0f; j += 0.25f) {
			float x = (_imgViewX / -200.0f) + ((pixelX + i) / 100.0f);
			float y = (_imgViewY / -200.0f) + ((pixelY + j) / 100.0f);
			glm::vec3 start = _position + (x * _right) + (y * _up);
			rays.push_back(Ray(_viewpoint, start - _viewpoint, _renderDist));
		}
	}
	return rays;
}

#ifdef _DEBUG
void PerspCamera::debug() const {
	printf("viewpoint = (%f, %f, %f)\n", _viewpoint.x, _viewpoint.y, _viewpoint.z);
	Camera::debug();
}
#endif
