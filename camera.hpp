#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "utils.hpp"

struct camera {
	vec3 origin;

	vec3 horizontal;
	vec3 vertical;
	vec3 focal_length;
	vec3 lower_left_corner;
	
	vec3 back;
	vec3 right;
	vec3 up;

	double lens_radius;

	double open_time;
	double close_time;

	camera(
		vec3 origin,
		vec3 lookat,
		vec3 world_up,
		double vertical_fov,
		double aspect_ratio,
		double aperture,
		double focus_distance,
		double open_time = 0.0,
		double close_time = 0.0
	) : origin{origin}, open_time{open_time}, close_time{close_time} {
		back = unit_vector(origin - lookat);
		right = unit_vector(cross(world_up, back));
		up = cross(back, right);

		const auto v_height = 2 * tan(vertical_fov / 2);
		const auto v_width = v_height * aspect_ratio;

		constexpr auto FOCAL_LENGTH = 1;

		horizontal = focus_distance * v_width * right;
		vertical = focus_distance * v_height * up;
		focal_length = focus_distance * FOCAL_LENGTH * -back;

		lower_left_corner = origin + focal_length - (horizontal / 2) - (vertical / 2);

		lens_radius = aperture / 2;
	}

	ray shoot_ray(double h, double v) const {
		vec3 random_start = lens_radius * random_in_unit_disk();
		vec3 random_origin = origin + right * random_start.x + up * random_start.y;
		vec3 position = h * horizontal + v * vertical;
		double random_time = random_double(open_time, close_time);

		return ray(random_origin, lower_left_corner - random_origin + position, random_time);
	}
};
