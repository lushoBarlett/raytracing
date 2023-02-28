#pragma once

#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "utils.hpp"

struct lambertian : material {

	vec3 color;

	lambertian(const vec3& color) : color(color) {}

	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const override {
		vec3 direction = info.normal + random_unit_vector();
		if (direction.zero())
			direction = info.normal;

		attenuation = color;
		scattered = ray(info.point, direction, r_in.time);

		return true;
	}
};
