#pragma once

#ifndef CAMERA_H
#define CAMERA_H 1

class Camera;
class OrthoCamera;
class PerspCamera;

#include "glm.hpp"
#include "Entity.h"
#include "Ray.h"

/*
For image view width and height, 100 corresponds to 1 in the global coordinate system.
So setting camera x to 200 and y to 100 will result in a camera with an image view of x dimension 2 and y dimension 1.
*/

class Camera : public Entity {
protected:
	int _imgViewX;
	int _imgViewY;
	float _renderDist;
public:
	Camera(const glm::vec3& position, int x, int y, float renderDist = INFINITY);
	virtual ~Camera();

	virtual Ray getRay(int pixelX, int pixelY) const = 0;
#ifdef _DEBUG
	virtual void debug() const;
#endif
};

class OrthoCamera : public Camera {
protected:
public:
	OrthoCamera(const glm::vec3& position, int x, int y, float renderDist = INFINITY);
	
	virtual Ray getRay(int pixelX, int pixelY) const;
};

class PerspCamera : public Camera {
protected:
	glm::vec3 _viewpoint;
	float _focalDist;
public:
	PerspCamera(const glm::vec3& position, float focalDist, int x, int y, float renderDist = INFINITY);
	
	virtual PerspCamera& setOrientation(float yaw, float pitch, float roll = 0.0f);
	virtual Ray getRay(int pixelX, int pixelY) const;
#ifdef _DEBUG
	virtual void debug() const;
#endif
};

#endif