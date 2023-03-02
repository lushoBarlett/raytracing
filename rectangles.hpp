#pragma once

#include "hittable.hpp"
#include "aabb.hpp"
#include "ray.hpp"

struct rect : hittable {

	double k, left, right, down, up;
	std::shared_ptr<material> texture;

	rect() {}

	void assign_corners(double centerw, double centerh, double width, double height) {
		width /= 2;
		height /= 2;

		left = centerw - width;
		right = centerw + width;

		down = centerh - height;
		up = centerh + height;
	}

	void calculate_uv(double pointw, double pointh, hit& info) const {
		info.u = (pointw - left) / (right - left);
		info.v = (pointh - down) / (up - down);
	}

	bool out_of_bounds(const ray& r, double t_min, double t_max, size_t plane, double& hit, vec3& point) const {
		const auto t = (k - r.origin[plane]) / r.direction[plane];

		if (t < t_min || t_max < t)
			return true;

		const auto p = r.at(t);

		const auto w = p[(plane + 1) % 3];
		const auto h = p[(plane + 2) % 3];

		if (w < left || right < w || h < down || up < h)
			return true;

		hit = t;
		point = p;	

		return false;
	}
};

struct xy_rect : rect {

	xy_rect(vec3 center, double width, double height, std::shared_ptr<material> texture) {
		this->texture = texture;
		k = center.z;
		assign_corners(center.x, center.y, width, height);
	}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {
		if (out_of_bounds(r, t_min, t_max, 2, info.parameter, info.point))
			return false;

		calculate_uv(info.point.x, info.point.y, info);
		info.face_determination(r, vec3(0, 0, 1));
		info.material_pointer = texture;
		return true;
	};

	virtual bool bounding_box(double t_min, double t_max, aabb& output_box) const override {
		output_box = aabb(vec3(left, down, k - 0.001), vec3(right, up, k + 0.001));
		return true;
	};
};

struct yz_rect : rect {

	yz_rect(vec3 center, double width, double height, std::shared_ptr<material> texture) {
		this->texture = texture;
		k = center.x;
		assign_corners(center.y, center.z, width, height);
	}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {
		if (out_of_bounds(r, t_min, t_max, 0, info.parameter, info.point))
			return false;

		calculate_uv(info.point.y, info.point.z, info);
		info.face_determination(r, vec3(1, 0, 0));
		info.material_pointer = texture;
		return true;
	};

	virtual bool bounding_box(double t_min, double t_max, aabb& output_box) const override {
		output_box = aabb(vec3(k - 0.001, left, down), vec3(k + 0.001, right, up));
		return true;
	};
};

struct xz_rect : rect {

	xz_rect(vec3 center, double width, double height, std::shared_ptr<material> texture) {
		this->texture = texture;
		k = center.y;
		assign_corners(center.x, center.z, width, height);
	}

	virtual bool test_hit(const ray& r, double t_min, double t_max, hit& info) const override {
		if (out_of_bounds(r, t_min, t_max, 1, info.parameter, info.point))
			return false;

		calculate_uv(info.point.x, info.point.z, info);
		info.face_determination(r, vec3(0, 1, 0));
		info.material_pointer = texture;
		return true;
	};

	virtual bool bounding_box(double t_min, double t_max, aabb& output_box) const override {
		output_box = aabb(vec3(left, k - 0.001, down), vec3(right, k + 0.001, up));
		return true;
	};
};