#pragma once

#include "utils.hpp"

struct perlin {

	static const int SIZE = 256;
	vec3* random;
	int* x_permutation;
	int* y_permutation;
	int* z_permutation;

	perlin() {
		random = new vec3[SIZE];

		for (int i = 0; i < SIZE; i++)
			random[i] = unit_vector(vec3_random(-1, 1));

		x_permutation = make_permutation();
		y_permutation = make_permutation();
		z_permutation = make_permutation();
	}

	double at(const vec3& p) const {
		double u = p.x - floor(p.x);
		double v = p.y - floor(p.y);
		double w = p.z - floor(p.z);

		int i = static_cast<int>(floor(p.x));
		int j = static_cast<int>(floor(p.y));
		int k = static_cast<int>(floor(p.z));

		vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = random[x_permutation[(i + di) & 0xFF] ^
					                       y_permutation[(j + dj) & 0xFF] ^
					                       z_permutation[(k + dk) & 0xFF]];
		
		return trilinear_interpolation(c, u, v, w);
	}

	double turbulence(const vec3& p, int depth = 7) const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++) {
			accum += weight * at(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}

		return fabs(accum);
	}

	static double trilinear_interpolation(vec3 c[2][2][2], double u, double v, double w) {
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		double accum = 0.0;
		for (size_t i = 0; i < 2; i++)
			for (size_t j = 0; j < 2; j++)
				for (size_t k = 0; k < 2; k++) {

					vec3 weight_v(u - i, v - j, w - k);

					accum += (i * uu + (1 - i) * (1 - uu)) *
					         (j * vv + (1 - j) * (1 - vv)) *
					         (k * ww + (1 - k) * (1 - ww)) *
					         dot(c[i][j][k], weight_v);
				}

		return accum;
	}

	int* make_permutation() {
		auto permutation = new int[SIZE];

		for (size_t i = 0; i < SIZE; i++)
			permutation[i] = i;

		for (size_t i = SIZE - 1; i > 0; i--)
			std::swap(permutation[i], permutation[random_int(0, i)]);

		return permutation;
	}

	~perlin() {
		delete[] random;
		delete[] x_permutation;
		delete[] y_permutation;
		delete[] z_permutation;
	}
};
