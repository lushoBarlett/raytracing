#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "utils.hpp"
#include "texture.hpp"

struct hit;

struct material {
	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const = 0;
};

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

struct metal : material {

	vec3 color;
	double fuzzyness;

	metal(const vec3& color, double fuzz) : color(color), fuzzyness(fuzz < 1 ? fuzz : 1) {}

	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const override {
		vec3 direction = reflect(unit_vector(r_in.direction), info.normal);
		attenuation = color;
		scattered = ray(info.point, direction + fuzzyness * random_in_unit_sphere(), r_in.time);

		return (dot(scattered.direction, info.normal) > 0);
	}
};

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
