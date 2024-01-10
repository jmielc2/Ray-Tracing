#include "../stdafx.h"
#include "Surface.h"

/* Abstract Surface Class */

Surface::Surface(const glm::vec3& position, Surface::SURFACE_TYPES type, float reflectivity) : Entity(position), _phongExp(type), _reflectivity(reflectivity) {
	return;
}

Surface::~Surface() {
	return;
}

/* Ground Surface Class */

Ground::Ground(const glm::vec3& color, Surface::SURFACE_TYPES type, float reflectivity) : Surface(glm::vec3(0.0f, 0.0f, 0.0f), type, reflectivity), _baseColor(color) {
	return;
}

float Ground::getIntersectionParam(const Ray& ray) const {
	float startY = ray.getStart().y;
	float dirY = ray.getDirection().y;
	if (std::abs(dirY) <= 0.001 || dirY * startY >= 0.0f) {
		return -1.0f;
	}
	float t = (-1.0f * startY) / dirY;
	if (ray.isWithinBounds(t)) {
		return t;
	}
	return -1.0f;
}

glm::vec3 Ground::getBaseColor(const glm::vec3& point) const {
	int x = int(glm::floor(point.x));
	int y = int(glm::floor(point.z));
#ifdef _DEBUG
	if (!x && !y) {
		return glm::vec3(0.0f, 0.0f, 255.0f);
	} else if ((x & 1) ^ (y & 1)) {
		return _baseColor;
	}
#else
	if ((x & 1) ^ (y & 1)) {
		return _baseColor;
	}
#endif
	return glm::vec3(70.0f, 70.0f, 70.0f);
}

/* Sphere Surface Class */

Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color, Surface::SURFACE_TYPES type, float reflectivity) : Surface(center, type, reflectivity), _radius(radius), _baseColor(color) {
	return;
}

float Sphere::getIntersectionParam(const Ray& ray) const {
	const glm::vec3 dir = ray.getDirection();
	const glm::vec3 centerToRay = ray.getStart() - _position;
	double discriminant = std::pow(glm::dot(dir, centerToRay), 2.0f) - (std::pow(glm::length(centerToRay), 2.0) - std::pow(_radius, 2.0));
	if (discriminant < 0.0) {
		return -1.0f;
	}
	float a = glm::dot(-1.0f * dir, centerToRay);
	float b = std::sqrt(float(discriminant));
	float t = std::min(a + b, a - b);
	if (ray.isWithinBounds(t)) {
		return t;
	}
	return -1.0f;
}

glm::vec3 Sphere::getNormal(const glm::vec3& point) const {
	return glm::normalize(point - _position);
}