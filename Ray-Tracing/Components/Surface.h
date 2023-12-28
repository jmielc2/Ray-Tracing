#pragma once

#ifndef SURFACE_H
#define SURFACE_H 1

class Surface;
class Sphere;

#include <glm.hpp>
#include "Entity.h"
#include "Ray.h"

class Surface : public Entity {
protected:
public:
	virtual float getIntersectionParam(const Ray& ray) const = 0;
	virtual glm::vec3 getNormal(const glm::vec3& point) const = 0;
};

class Sphere : public Surface {
protected:
public:
	virtual float getIntersectionParam(const Ray& ray) const;
	virtual glm::vec3 getNormal(const glm::vec3& point) const;
};

#endif
