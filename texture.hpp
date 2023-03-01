#pragma once

#include <memory>

#include "utils.hpp"

struct texture {
	virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

struct solid_color : texture {

	vec3 color;

	solid_color() {}
	solid_color(const vec3& c) : color{c} {}

	solid_color(double red, double green, double blue)
	    : solid_color(vec3(red, green, blue)) {}

	virtual vec3 value(double u, double v, const vec3& p) const override {
		return color;
	}
};

struct checker_texture : texture {

	std::shared_ptr<texture> even;
	std::shared_ptr<texture> odd;

	checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
	    : even{even}, odd{odd} {}

	checker_texture(const vec3& even_color, const vec3& odd_color)
	    : even{std::make_shared<solid_color>(even_color)}, odd{std::make_shared<solid_color>(odd_color)} {}

	virtual vec3 value(double u, double v, const vec3& p) const override {

		auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);

		return (sines < 0 ? odd : even)->value(u, v, p);
	}
};
