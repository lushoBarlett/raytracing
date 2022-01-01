#pragma once

#include "ray.hpp"

struct hit {
	double parameter;
	vec3 point;
	vec3 normal;
	bool front_face;

	inline void face_determination(const ray& r, const vec3& out_normal) {
		front_face = dot(r.direction, out_normal) < 0;
		normal = front_face ? out_normal : -out_normal;
	}
};

struct hittable {
	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const = 0;
};
