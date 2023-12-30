#pragma once

#ifndef SURFACE_H
#define SURFACE_H 1

class Surface;
class Sphere;

#include <SDL.h>
#include <glm.hpp>
#include "Entity.h"
#include "Ray.h"

class Surface : public Entity {
protected:
	Surface(const glm::vec3& position);
public:
	virtual glm::vec3 getNormal(const glm::vec3& point) const = 0;
	virtual float getIntersectionParam(const Ray& ray) const = 0;
	virtual glm::vec3 getBaseColor(const glm::vec3& point) const = 0;
};

class Ground : public Surface {
protected:
	glm::vec3 _baseColor;
public:
	Ground(const glm::vec3& color);

	virtual inline glm::vec3 getNormal(const glm::vec3& point) const { return glm::vec3(0.0f, 1.0f, 0.0f); }
	virtual float getIntersectionParam(const Ray& ray) const;
	virtual glm::vec3 getBaseColor(const glm::vec3& point) const;
};

class Sphere : public Surface {
protected:
	float _radius;
	glm::vec3 _baseColor;
public:
	Sphere(const glm::vec3& center, float radius, const glm::vec3& color);

	virtual glm::vec3 getNormal(const glm::vec3& point) const;
	virtual float getIntersectionParam(const Ray& ray) const;
	virtual inline glm::vec3 getBaseColor(const glm::vec3& point) const { return _baseColor; }
};

#endif
