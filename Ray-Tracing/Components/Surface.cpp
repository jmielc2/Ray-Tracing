#include "../stdafx.h"
#include "Surface.h"

float Sphere::getIntersectionParam(const Ray& ray) const {
	return -1.0f;
}

glm::vec3 Sphere::getNormal(const glm::vec3& point) const {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}