#include "../stdafx.h"
#include "Light.h"

Light::Light(const glm::vec3& position, float intensity, const glm::vec3& color) : Entity(position), _lightColor(color), _intensity(intensity) {
	return;
}

Light::~Light() {
	return;
}