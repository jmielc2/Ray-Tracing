#pragma once

#ifndef CAMERA_H
#define CAMERA_H 1

class Camera;
class OrthoCamera;
class PerspCamera;

#include "glm.hpp"
#include "Entity.h"
#include "Ray.h"

class Camera : public Entity {
protected:
	int _imgViewX;
	int _imgViewY;
public:
	Camera(const glm::vec3& position);
	Camera(const glm::vec3& position, int x, int y);

	virtual Ray getRay(int pixelX, int pixelY) const = 0;
	void debug() const;
};

class OrthoCamera : public Camera {
protected:
public:
	OrthoCamera();
	OrthoCamera(const glm::vec3& position);
	OrthoCamera(const glm::vec3& position, int x, int y);
	
	virtual Ray getRay(int pixelX, int pixelY) const;
};

class PerspCamera : public Camera {
protected:
	glm::vec3 _viewpoint;
	float _focalDist;
public:
	PerspCamera();
	PerspCamera(const glm::vec3& position, float focalDist);
	PerspCamera(const glm::vec3& position, float focalDist, int x, int y);
	
	PerspCamera& setOrientation(float yaw, float pitch);
	virtual Ray getRay(int pixelX, int pixelY) const;
	void debug() const;
};

#endif