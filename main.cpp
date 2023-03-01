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

constexpr int SCENE = 1;

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

std::shared_ptr<lambertian> make_checker_texture() {
	return std::make_shared<lambertian>(
	    std::make_shared<checker_texture>(
	        vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9)));
}

std::shared_ptr<lambertian> make_random_albedo() {
	return std::make_shared<lambertian>(
	    vec3_random() * vec3_random());
}

std::shared_ptr<dielectric> make_dielectric() {
	return std::make_shared<dielectric>(1.5);
}

std::shared_ptr<sphere> make_small_sphere(vec3 center, std::shared_ptr<material> material) {
	return std::make_shared<sphere>(center, 0.2, material);
}

hittable_list random_scene() {
	hittable_list scene;

	scene.objects.push_back(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, make_checker_texture()));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			auto material_probability = random_double();

			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (material_probability < 0.8) {
					auto moving_sphere = make_small_sphere(center, make_random_albedo());
					moving_sphere->velocity = vec3(0.0, 0.1, 0.0);
					scene.objects.push_back(moving_sphere);

				} else if (material_probability < 0.95) {
					auto albedo = vec3_random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					auto metal_material = std::make_shared<metal>(albedo, fuzz);
					scene.objects.push_back(make_small_sphere(center, metal_material));

				} else {
					scene.objects.push_back(make_small_sphere(center, make_dielectric()));
				}
			}
		}
	}

	scene.objects.push_back(std::make_shared<sphere>(vec3(0, 1, 0), 1.0, make_dielectric()));

	auto material2 = std::make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
	scene.objects.push_back(std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
	scene.objects.push_back(std::make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

	return scene;
}

hittable_list two_spheres() {
	hittable_list scene;

	scene.objects.push_back(std::make_shared<sphere>(vec3(0, -10, 0), 10, make_checker_texture()));
	scene.objects.push_back(std::make_shared<sphere>(vec3(0,  10, 0), 10, make_checker_texture()));

	return scene;
}

vec3 screen[WIDTH][HEIGHT];

hittable_list choose_scene(int scene) {
	switch (scene) {
		default:
		case 1: return random_scene();
		case 2: return two_spheres();
	}
}

int main() {
	double start_time = 0;
	double end_time = 1;
	
	bvh_node bvh(choose_scene(SCENE), start_time, end_time);

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

	std::cout << "P6\n" << WIDTH << ' ' << HEIGHT << "\n255\n";
	for (int j = HEIGHT - 1; j >= 0; --j)
		for (int i = 0; i < WIDTH; ++i)
			write_color(std::cout, screen[i][j], SAMPLES);

	std::cerr << "\nDone.\n";
}
