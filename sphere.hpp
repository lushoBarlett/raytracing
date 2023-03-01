#pragma once

#include <cmath>

#include "aabb.hpp"
#include "ray.hpp"
#include "hittable.hpp"

struct sphere : hittable {
	
	vec3 center;
	vec3 velocity;
	double radius;
	std::shared_ptr<material> material_pointer;

	sphere() = default;
	sphere(const vec3& center, double radius, std::shared_ptr<material> material_pointer)
	    : center{center}, radius{radius}, material_pointer{material_pointer} {
		velocity = vec3(0, 0, 0);
	}

	static void get_sphere_uv(const vec3& p, double& u, double& v) {
		auto theta = acos(-p.y);
		auto phi = atan2(-p.z, p.x) + M_PI;

		u = phi / (2 * M_PI);
		v = theta / M_PI;
	}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {

		const auto current_center = center + r.time * velocity;

		const auto distance = r.origin - current_center;

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
		vec3 out_normal = unit_vector(info.point - current_center);
		info.face_determination(r, out_normal);
		get_sphere_uv(out_normal, info.u, info.v);
		info.material_pointer = material_pointer;
		
		return true;
	}

	virtual bool bounding_box(double t_min, double t_max, aabb& output_box) const override {

		const auto first_center = center + t_min * velocity;
		const auto second_center = center + t_max * velocity;

		const auto first_box = aabb(first_center - vec3(radius, radius, radius), first_center + vec3(radius, radius, radius));
		const auto second_box = aabb(second_center - vec3(radius, radius, radius), second_center + vec3(radius, radius, radius));

		output_box = surrounding_box(first_box, second_box);

		return true;
	}
};
