#include <iostream>
#include <memory>
#include <random>

#include "bvh_node.hpp"
#include "vec3.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"

constexpr double ASPECT_RATIO = (double)3 / 2;
constexpr int WIDTH = 700;
constexpr int HEIGHT = WIDTH / ASPECT_RATIO;

constexpr int SAMPLES = 20;
constexpr int MAX_RAY_DEPTH = 5;

vec3 ray_color(const ray& r, const hittable& world, int depth = 1) {
	const vec3 white(1, 1, 1);
	const vec3 lightblue(0.5, 0.7, 1);

	if (depth <= 0)
		return vec3(0, 0, 0);

	hit info;
	if (world.test_hit(r, 0.01, INFINITY, info)) {
		vec3 attenuation;
		ray scattered;
		if (info.material_pointer->scatter(r, info, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return vec3(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction);
	double t = unit_map(unit_direction.y);
	return vec3_lerp(white, lightblue, t);
}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
	world.objects.push_back(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3_random() * vec3_random();
					sphere_material = std::make_shared<lambertian>(albedo);
					auto moving_sphere = std::make_shared<sphere>(center, 0.2, sphere_material);
					moving_sphere->velocity = vec3(0.0, 0.1, 0.0);
					world.objects.push_back(moving_sphere);
				} else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3_random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.objects.push_back(std::make_shared<sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.objects.push_back(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.objects.push_back(std::make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
	world.objects.push_back(std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
	world.objects.push_back(std::make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

	return world;
}

vec3 screen[WIDTH][HEIGHT];

int main() {
	hittable_list world = random_scene();

	double start_time = 0;
	double end_time = 1;
	
	bvh_node bvh(world, start_time, end_time);

	camera c(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), M_PI / 8, ASPECT_RATIO, 0.1, 10);

	int scanlines = HEIGHT - 1;

	#pragma omp parallel for
	for (int j = HEIGHT - 1; j >= 0; --j) {

		if (scanlines % 16 == 0)
			std::cerr << "\rScanlines remaining: " << scanlines << ' ' << std::flush;

		for (int i = 0; i < WIDTH; ++i) {

			vec3 color(0, 0, 0);
			for (size_t s = 0; s < SAMPLES; s++) {
				double h = (i + random_double()) / (WIDTH - 1);
				double v = (j + random_double()) / (HEIGHT - 1);
				ray r = c.shoot_ray(h, v);
				color += ray_color(r, bvh, MAX_RAY_DEPTH);
			}
			
			screen[i][j] = color;
		}

		#pragma omp atomic
		scanlines--;
	}

	std::cerr << "\rScanlines remaining: 0\nWriting..." << std::flush;

	std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
	for (int j = HEIGHT - 1; j >= 0; --j)
		for (int i = 0; i < WIDTH; ++i)
			write_color(std::cout, screen[i][j], SAMPLES);

	std::cerr << "\nDone.\n";
}
