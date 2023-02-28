#pragma once

#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "utils.hpp"

struct dielectric : material {

	double index;

	dielectric(double index) : index(index) {}

	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const override {
		double refraction_ratio = info.front_face ? (1 / index) : index;

		vec3 unit_direction = unit_vector(r_in.direction);
		double cos_theta = fmin(dot(-unit_direction, info.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		vec3 direction =
			refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double()
			? reflect(unit_direction, info.normal)
			: refract(unit_direction, info.normal, refraction_ratio);

		attenuation = vec3(1, 1, 1);
		scattered = ray(info.point, direction, r_in.time);

		return true;
	}

	// Schlick's approximation for reflectance
	static double reflectance(double cos_theta, double refraction) {
		auto r0 = (1 - refraction) / (1 + refraction);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cos_theta), 5);
	}
};

