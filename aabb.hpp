#pragma once

#include <algorithm>

#include "ray.hpp"
#include "vec3.hpp"

struct aabb {
	vec3 minimum;
	vec3 maximum;

	aabb() {}
	aabb(const vec3& a, const vec3& b) : minimum{a}, maximum{b} {}

	inline bool hit(const ray& r, double t_min, double t_max) const {

		for (int a = 0; a < 3; a++) {

			auto inverse_direction = 1.0f / r.direction[a];
			auto t0 = (minimum[a] - r.origin[a]) * inverse_direction;
			auto t1 = (maximum[a] - r.origin[a]) * inverse_direction;

			if (inverse_direction < 0.0f)
				std::swap(t0, t1);

			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;

			if (t_max <= t_min)
				return false;
		}

		return true;
	}
};

aabb surrounding_box(aabb box1, aabb box2) {
	vec3 minimum(fmin(box1.minimum.x, box2.minimum.x),
	             fmin(box1.minimum.y, box2.minimum.y),
	             fmin(box1.minimum.z, box2.minimum.z));

	vec3 maximum(fmax(box1.maximum.x, box2.maximum.x),
	             fmax(box1.maximum.y, box2.maximum.y),
	             fmax(box1.maximum.z, box2.maximum.z));

	return aabb(minimum, maximum);
}