#pragma once

#include "vec3.hpp"

struct ray {
	vec3 origin;
	vec3 direction;

	ray() = default;
	ray(const vec3& origin, const vec3& direction) : origin{origin}, direction{direction} {}

	vec3 at(float c) const {
		return origin + c * direction;
	}
};
