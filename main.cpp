#include <iostream>
#include <memory>
#include <random>

#include "vec3.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"

constexpr double RATIO = (double)16 / 9;
constexpr int WIDTH = 700;
constexpr int HEIGHT = WIDTH / RATIO;

constexpr int SAMPLES = 100;

// NOTE:   -1 <= y <= 1
// We want  0 <= t <= 1 
inline double unit_map(double x) {
	return 0.5 * (x + 1);
}

inline vec3 vec3_unit_map(const vec3& v) {
	return 0.5 * (v + vec3(1, 1, 1));
}

inline vec3 vec3_lerp(const vec3& origin, const vec3& destination, double t) {
	return (1 - t) * origin + t * destination;
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	return distribution(generator);
}

vec3 ray_color(const ray& r, const hittable& world) {
	const vec3 white(1, 1, 1);
	const vec3 lightblue(0.5, 0.7, 1);

	hit info;
	if (world.test_hit(r, 0, INFINITY, info))
		return vec3_unit_map(info.normal);

	vec3 unit_direction = unit_vector(r.direction);
	double t = unit_map(unit_direction.y);
	return vec3_lerp(white, lightblue, t);
}

int main() {

	std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

	hittable_list world;
	world.objects.push_back(std::make_shared<sphere>(vec3(0, 0, -1), 0.5));
	world.objects.push_back(std::make_shared<sphere>(vec3(0, -100.5, -1), 100));

	camera c;

	for (int j = HEIGHT - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < WIDTH; ++i) {
			
			vec3 color(0, 0, 0);
			for (size_t s = 0; s < SAMPLES; s++) {
				double h = (i + random_double()) / (WIDTH - 1);
				double v = (j + random_double()) / (HEIGHT - 1);
				ray r = c.shoot_ray(h, v);
				color += ray_color(r, world);
			}
			
			write_color(std::cout, color, SAMPLES);
		}
	}
	std::cerr << "\nDone.\n";
}
