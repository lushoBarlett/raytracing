#pragma once

#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "utils.hpp"
#include "texture.hpp"

struct lambertian : material {

	std::shared_ptr<texture> albedo;

	lambertian(const vec3& color) : albedo{std::make_shared<solid_color>(color)} {}
	lambertian(std::shared_ptr<texture> albedo) : albedo{albedo} {}

	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const override {
		vec3 direction = info.normal + random_unit_vector();
		if (direction.zero())
			direction = info.normal;

		attenuation = albedo->value(info.u, info.v, info.point);
		scattered = ray(info.point, direction, r_in.time);

		return true;
	}
};
