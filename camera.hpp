#pragma once

#include "vec3.hpp"
#include "ray.hpp"

struct camera {
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 focal_length;
	vec3 lower_left_corner;

	camera() {
		constexpr auto ASPECT_RATIO = (double)16 / 9;
		constexpr auto V_HEIGHT = 2;
		constexpr auto V_WIDTH = ASPECT_RATIO * V_HEIGHT;
		constexpr auto FOCAL_LENGTH = 1;

		origin = vec3(0, 0, 0);

		horizontal = vec3(V_WIDTH, 0, 0);
		vertical = vec3(0, V_HEIGHT, 0);
		focal_length = vec3(0, 0, FOCAL_LENGTH);

		lower_left_corner = origin - focal_length - (horizontal / 2) - (vertical / 2);
	}

	ray shoot_ray(double h, double v) const {
		return ray(origin, lower_left_corner - origin + h * horizontal + v * vertical);
	}
};
