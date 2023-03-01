#pragma once

#include <memory>

#include "aabb.hpp"
#include "ray.hpp"

struct material;

struct hit {
	double parameter;
	double u;
	double v;
	vec3 point;
	vec3 normal;
	bool front_face;
	std::shared_ptr<material> material_pointer;

	inline void face_determination(const ray& r, const vec3& out_normal) {
		front_face = dot(r.direction, out_normal) < 0;
		normal = front_face ? out_normal : -out_normal;
	}
};

struct hittable {
	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const = 0;
	virtual bool bounding_box(double t_min, double t_max, aabb& output_box) const = 0;
};
