#pragma once

#ifndef SURFACE_H
#define SURFACE_H 1

class Surface;
class Sphere;

#include <SDL.h>
#include <glm/glm.hpp>
#include "Entity.h"
#include "Ray.h"

class Surface : public Entity {
public:
	enum SURFACE_TYPES { EGGSHELL = 10, SHINY = 100, GLOSSY = 1000, MIRROR = 10000 };
protected:
	int _phongExp;
	float _reflectivity;
	Surface(const glm::vec3& position, Surface::SURFACE_TYPES type = Surface::SURFACE_TYPES::SHINY, float reflectivity = 0.0f);
public:
	virtual ~Surface();
	
	inline float getReflectivity() const { return _reflectivity; }
	inline float getPhongExponent() const { return float(_phongExp); }
	virtual glm::vec3 getNormal(const glm::vec3& point) const = 0;
	virtual float getIntersectionParam(const Ray& ray) const = 0;
	virtual glm::vec3 getBaseColor(const glm::vec3& point) const = 0;
};

class Ground : public Surface {
protected:
	glm::vec3 _baseColor;
public:
	Ground(const glm::vec3& color, Surface::SURFACE_TYPES type = Surface::SURFACE_TYPES::GLOSSY, float reflectivity = 0.25f);

	virtual inline glm::vec3 getNormal(const glm::vec3& point) const { return glm::vec3(0.0f, 1.0f, 0.0f); }
	virtual float getIntersectionParam(const Ray& ray) const;
	virtual glm::vec3 getBaseColor(const glm::vec3& point) const;
};

class Sphere : public Surface {
protected:
	float _radius;
	glm::vec3 _baseColor;
public:
	Sphere(const glm::vec3& center, float radius, const glm::vec3& color, Surface::SURFACE_TYPES type = Surface::SURFACE_TYPES::SHINY, float reflectivity = 0.0f);

	virtual glm::vec3 getNormal(const glm::vec3& point) const;
	virtual float getIntersectionParam(const Ray& ray) const;
	virtual inline glm::vec3 getBaseColor(const glm::vec3& point) const { return _baseColor; }
};

#endif
