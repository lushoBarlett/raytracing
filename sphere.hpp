#pragma once

#include "ray.hpp"
#include "hittable.hpp"

struct sphere : hittable {
	
	vec3 center;
	double radius;
	std::shared_ptr<material> material_pointer;

	sphere() = default;
	sphere(const vec3& center, double radius, std::shared_ptr<material> material_pointer)
	    : center{center}, radius{radius}, material_pointer{material_pointer} {}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {
		const auto distance = r.origin - center;

		// NOTE: length_squared is equivalent to dot product with itself
		const auto a = r.direction.length_squared();
		const auto hb = dot(r.direction, distance);
		const auto c = distance.length_squared() - radius * radius;

		const auto discriminant = hb * hb - a * c;
		
		if (discriminant < 0)
			return false;

		const auto discriminant_sqrt = std::sqrt(discriminant);

		auto root = (-hb - discriminant_sqrt) / a;
		if (root < t_min || t_max < root) {
			root = (-hb + discriminant_sqrt) / a;
			if (root < t_min || t_max < root)
				return false;
		}

		info.parameter = root;
		info.point = r.at(info.parameter);
		vec3 out_normal = unit_vector(info.point - center);
		info.face_determination(r, out_normal);
		info.material_pointer = material_pointer;
		
		return true;
	}
};
