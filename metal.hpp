#pragma once

#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "utils.hpp"

struct metal : material {

	vec3 color;
	double fuzzyness;

	metal(const vec3& color, double fuzz) : color(color), fuzzyness(fuzz < 1 ? fuzz : 1) {}

	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const override {
		vec3 direction = reflect(unit_vector(r_in.direction), info.normal);
		attenuation = color;
		scattered = ray(info.point, direction + fuzzyness * random_in_unit_sphere());

		return (dot(scattered.direction, info.normal) > 0);
	}
};
