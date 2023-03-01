#pragma once

#include <memory>
#include <vector>

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "utils.hpp"

inline bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis) {
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.minimum[axis] < box_b.minimum[axis];
}

bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

struct bvh_node : hittable {
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb box;

	bvh_node() = default;

	bvh_node(const hittable_list& list, double t_min, double t_max)
	    : bvh_node(list.objects, 0, list.objects.size(), t_min, t_max) {}

	bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end, double t_min, double t_max) {
		auto objects = src_objects;  // Create a modifiable array of the source scene objects

		int axis = random_int(0, 2);

		auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

		size_t size = end - start;

		if (size == 1) {
			left = right = objects[start];

		} else if (size == 2) {
			if (comparator(objects[start], objects[start + 1])) {
				left = objects[start];
				right = objects[start + 1];
			} else {
				left = objects[start + 1];
				right = objects[start];
			}

		} else {
			std::sort(objects.begin() + start, objects.begin() + end, comparator);

			auto mid = (start + end) / 2;
			left = std::make_shared<bvh_node>(objects, start, mid, t_min, t_max);
			right = std::make_shared<bvh_node>(objects, mid, end, t_min, t_max);
		}

		aabb box_left, box_right;

		if (!left->bounding_box(t_min, t_max, box_left) || !right->bounding_box(t_min, t_max, box_right))
			std::cerr << "No bounding box in bvh_node constructor.\n";

		box = surrounding_box(box_left, box_right);
	};

	bool test_hit(const ray& r, double t_min, double t_max, hit& hit_record) const override {
		if (!box.hit(r, t_min, t_max))
			return false;

		bool hit_left = left->test_hit(r, t_min, t_max, hit_record);
		bool hit_right = right->test_hit(r, t_min, hit_left ? hit_record.parameter : t_max, hit_record);

		return hit_left || hit_right;
	}

	bool bounding_box(double t_min, double t_max, aabb& output_box) const override {
		output_box = box;
		return true;
	}
};
