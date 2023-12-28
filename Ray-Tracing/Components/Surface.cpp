#include "../stdafx.h"
#include "Surface.h"

/* Abstract Surface Class */

Surface::Surface(const glm::vec3& position) : Entity(position) {
	return;
}

/* Ground Surface Class */

Ground::Ground(const glm::vec3& color) : Surface(glm::vec3(0.0f, 0.0f, 0.0f)), _baseColor(color) {
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

Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color) : Surface(center), _radius(radius), _baseColor(color) {
	return;
}

float Sphere::getIntersectionParam(const Ray& ray) const {
	return -1.0f;
}

glm::vec3 Sphere::getNormal(const glm::vec3& point) const {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}