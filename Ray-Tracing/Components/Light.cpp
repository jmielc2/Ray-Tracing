#include "../stdafx.h"
#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity) : Entity(position), _lightColor(color), _intensity(intensity) {
	return;
}