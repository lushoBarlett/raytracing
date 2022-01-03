#pragma once

#include "vec3.hpp"
#include "ray.hpp"

struct hit;

struct material {
	virtual bool scatter(const ray& r_in, const hit& info, vec3& attenuation, ray& scattered) const = 0;
};