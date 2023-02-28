#pragma once

#include "vec3.hpp"

struct ray {
	vec3 origin;
	vec3 direction;
	double time;

	ray() = default;
	ray(const vec3& origin, const vec3& direction, double time = 0.0)
	    : origin{origin}, direction{direction}, time{time}
	{}

	vec3 at(float c) const {
		return origin + c * direction;
	}
};
