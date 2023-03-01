#pragma once

#include <cmath>
#include <iostream>
#include <exception>
#include <cassert>

struct vec3 {
	float x;
	float y;
	float z;

	vec3() : x{0}, y{0}, z{0} {}
	vec3(float x, float y, float z) : x{x}, y{y}, z{z} {}
	vec3(const vec3& v) : x{v.x}, y{v.y}, z{v.z} {}

	float operator[](int s) const {
		switch (s) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: assert(0);
		}
	}

	vec3 operator-() const {
		return vec3(-x, -y, -z);
	}

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3& operator*=(const float c) {
		x *= c;
		y *= c;
		z *= c;
		return *this;
	}

	vec3& operator/=(const float c) {
		return *this *= 1 / c;
	}

	float length_squared() const {
		return x * x + y * y + z * z;
	}

	float length() const {
		return std::sqrt(length_squared());
	}

	bool zero() const {
		const auto s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(float t, const vec3& v) {
	return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3& v, float t) {
	return t * v;
}

inline vec3 operator/(const vec3& v, float t) {
	return (1 / t) * v;
}

inline float dot(const vec3& u, const vec3& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u.y * v.z - v.y * u.z,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(const vec3& v) {
	if (!v.length())
		throw std::logic_error("null vector has no unit vector");

	return v / v.length();
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}
