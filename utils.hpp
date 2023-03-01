#pragma once

#include <random>

#include "vec3.hpp"

// NOTE:   -1 <= y <= 1
// We want  0 <= t <= 1 
inline double unit_map(double x) {
	return 0.5 * (x + 1);
}

inline double random_double(double min = 0, double max = 1) {
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

inline vec3 vec3_unit_map(const vec3& v) {
	return 0.5 * (v + vec3(1, 1, 1));
}

inline vec3 vec3_lerp(const vec3& origin, const vec3& destination, double t) {
	return (1 - t) * origin + t * destination;
}

inline vec3 vec3_random(double min = 0, double max = 1) {
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 random_in_unit_sphere() {
	vec3 point;
	do
		point = vec3_random(-1, 1);	
	while(point.length_squared() >= 1);
	return point;
}

vec3 random_in_unit_disk() {
	vec3 point;
	do {
		point = vec3_random(-1, 1);
		point.z = 0;
	} while(point.length_squared() >= 1);
	return point;
}

vec3 random_unit_vector() {
	const auto not_normalized = random_in_unit_sphere();
	return unit_vector(not_normalized);
}

vec3 refract(const vec3& in, const vec3& n, double dielectric_ratio) {
	auto cos_theta = fmin(dot(-in, n), 1.0);
	vec3 out_perpendicular = dielectric_ratio * (in + cos_theta * n);
	vec3 out_parallel = -sqrt(fabs(1.0 - out_perpendicular.length_squared())) * n;
	return out_perpendicular + out_parallel;
}